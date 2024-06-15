/***************************************************************************
 *
 * OSAL
 * Copyright (C) 2023 / 2024  Antonio Salsi <passy.linux@zresa.it>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***************************************************************************/
#include "osal/streambuffer.hpp"

#include <stdlib.h>

namespace osal
{
inline namespace v1
{

stream_buffer::stream_buffer(size_t size, size_t trigger_size, error** error) OSAL_NOEXCEPT
{
    pthread_mutexattr_t mattr;
    pthread_condattr_t cattr;


    sb.buffer = new uint8_t(size);
    if (sb.buffer == nullptr)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("Out off memory.", error_type::OS_ENOMEM);
            OS_ERROR_PTR_SET_POSITION(*error);
        }
        return;
    }
    memset(sb.buffer, 0, size);

    pthread_condattr_init (&cattr);
    pthread_condattr_setclock (&cattr, CLOCK_MONOTONIC);
    pthread_cond_init (&sb.cond, &cattr);
    pthread_mutexattr_init (&mattr);
    pthread_mutexattr_setprotocol (&mattr, PTHREAD_PRIO_INHERIT);
    pthread_mutex_init (&sb.mutex, &mattr);

    sb.trigger_size = trigger_size;
    sb.r = 0;
    sb.w = 0;
    sb.end = 0;
    sb.count = 0;
    sb.size  = size;

}

stream_buffer::~stream_buffer()
{
    pthread_cond_destroy (&sb.cond);
    pthread_mutex_destroy (&sb.mutex);

    if(sb.buffer)
    {
        for(size_t i = 0; i < sb.size; i++)
        {
            sb.buffer[i] = 0;
        }
        reset();
        delete[] sb.buffer;
        sb.buffer = nullptr;
    }
}

size_t stream_buffer::send(const uint8_t *data, size_t size, uint64_t time, error** _error) OSAL_NOEXCEPT
{
    timespec ts{0};
    uint8_t error     = 0;
    uint64_t nsec = (uint64_t)time * 1'000'000;

    if(data == nullptr)
    {
        if(_error)
        {
            *_error = OS_ERROR_BUILD("Data nullptr", error_type::OS_EINVAL);
            OS_ERROR_PTR_SET_POSITION(*_error);
        }
        return false;
    }

    if(sb.count == sb.size)
    {
        if(_error)
        {
            *_error = OS_ERROR_BUILD("sb.count == sb.size", error_type::OS_EINVAL);
            OS_ERROR_PTR_SET_POSITION(*_error);
        }
        return false;
    }

    if (time != WAIT_FOREVER)
    {
        clock_gettime (CLOCK_MONOTONIC, &ts);
        nsec += ts.tv_nsec;

        ts.tv_sec += nsec / NSECS_PER_SEC;
        ts.tv_nsec = nsec % NSECS_PER_SEC;
    }

    pthread_mutex_lock (&sb.mutex);

    size_t ret = sb.count;
//    bool check_cond_wait_init = false;

    while (sb.count == sb.size)
    {
        if (time != WAIT_FOREVER)
        {
//            check_cond_wait_init = true;

            error = pthread_cond_timedwait (&sb.cond, &sb.mutex, &ts);
            if (error)
            {
                if(_error)
                {
                    switch (error_type(error))
                    {
                    case error_type::OS_ETIMEDOUT:
                        *_error = OS_ERROR_BUILD("The time specified by abstime to pthread_cond_timedwait() has passed.", error_type::OS_ETIMEDOUT);
                        OS_ERROR_PTR_SET_POSITION(*_error);
                        break;
                    case error_type::OS_EINVAL:
                        *_error = OS_ERROR_BUILD("The value specified by abstime is invalid.", error_type::OS_EINVAL);
                        OS_ERROR_PTR_SET_POSITION(*_error);
                        break;
                    case error_type::OS_EPERM:
                        *_error = OS_ERROR_BUILD("The mutex was not owned by the current thread at the time of the call.", error_type::OS_EPERM);
                        OS_ERROR_PTR_SET_POSITION(*_error);
                        break;
                    default:
                        *_error = OS_ERROR_BUILD("Unmanaged error", error);
                        OS_ERROR_PTR_SET_POSITION(*_error);
                        break;
                    }
                }
                goto timeout;
            }
        }
        else
        {
            error = pthread_cond_wait (&sb.cond, &sb.mutex);
            if (error)
            {
                if(_error)
                {
                    switch (error_type(error))
                    {
                    case error_type::OS_ETIMEDOUT:
                        *_error = OS_ERROR_BUILD("The time specified by abstime to pthread_cond_timedwait() has passed.", error_type::OS_ETIMEDOUT);
                        OS_ERROR_PTR_SET_POSITION(*_error);
                        break;
                    case error_type::OS_EINVAL:
                        *_error = OS_ERROR_BUILD("The value specified by abstime is invalid.", error_type::OS_EINVAL);
                        OS_ERROR_PTR_SET_POSITION(*_error);
                        break;
                    default:
                        *_error = OS_ERROR_BUILD("Unmanaged error", error);
                        OS_ERROR_PTR_SET_POSITION(*_error);
                        break;
                    }
                }
                goto timeout;
            }
        }
    }



    if( (sb.w + size) >= sb.size && sb.r > 0)
    {
        //I write the available space on buffer
        size_t data_to_write  = sb.size - sb.w;
        size_t data_override  = size - data_to_write;
        memcpy(sb.buffer + sb.w, data, data_to_write);
        sb.count += data_to_write;

        sb.end = sb.w;
        sb.w += data_to_write;

        //i write on already read data
        if(data_override <= sb.r)
        {
            //I can write all remaining data
            memcpy(sb.buffer, data + data_to_write, data_override);
            sb.w = data_override;
            sb.count += data_override;

        }
        else
        {
            //Partial writing, I trunk some data
            memcpy(sb.buffer, data + data_to_write, sb.r);
            sb.w = sb.r;
            sb.count += sb.r;
            error = 1;
        }
    }
    else if(sb.r == 0)
    {
        //not override
        if( (sb.w + size) <= sb.size)
        {
            memcpy(sb.buffer + sb.w, data, size);

            sb.count += size;

            sb.w  += size;
        }
        else
        {
            memcpy(sb.buffer + sb.w, data, sb.size - sb.w);

            sb.count += sb.size - sb.w;

            sb.w  += sb.size - sb.w;

        }
    }
    else if(sb.r > 0)
    {
        size_t size_available = abs(static_cast<long long>(sb.r - sb.w));

        if(size <= size_available)
        {
            memcpy(sb.buffer + sb.w, data, size);

            sb.count += size;

            sb.w  += size;
        }
        else
        {
            memcpy(sb.buffer + sb.r, data, size);

            sb.count += size;

            sb.r  += size;

            error = 1;
        }

    }


timeout:
    pthread_mutex_unlock (&sb.mutex);
    pthread_cond_signal (&sb.cond);

    //return (error == 0);
    return error ? 0 : sb.count - ret;
}

inline size_t stream_buffer::send_from_isr(const uint8_t *data, size_t size, uint64_t time, error **error) OSAL_NOEXCEPT
{
    return send(data, size, time, error);
}

size_t stream_buffer::receive(uint8_t *data, size_t size, uint64_t time, error **_error) OSAL_NOEXCEPT
{
    timespec ts{0};
    uint8_t error     = 0;
    uint64_t nsec = (uint64_t)time * 1'000'000;
    size_t already_received = 0;

    if(data == nullptr)
    {
        if(_error)
        {
            *_error = OS_ERROR_BUILD("Data nullptr", error_type::OS_EINVAL);
            OS_ERROR_PTR_SET_POSITION(*_error);
        }
        return false;
    }


    if (time != WAIT_FOREVER)
    {
        clock_gettime (CLOCK_MONOTONIC, &ts);
        nsec += ts.tv_nsec;

        ts.tv_sec += nsec / NSECS_PER_SEC;
        ts.tv_nsec = nsec % NSECS_PER_SEC;
    }

    pthread_mutex_lock (&sb.mutex);

//    bool check_cond_wait_init = false;
    while (sb.count < sb.trigger_size)
    {
        if (time != WAIT_FOREVER)
        {
//            check_cond_wait_init = true;

            error = pthread_cond_timedwait (&sb.cond, &sb.mutex, &ts);
            if (error)
            {
                if(_error)
                {
                    switch (error_type(error))
                    {
                    case error_type::OS_ETIMEDOUT:
                        *_error = OS_ERROR_BUILD("The time specified by abstime to pthread_cond_timedwait() has passed.", error_type::OS_ETIMEDOUT);
                        OS_ERROR_PTR_SET_POSITION(*_error);
                        break;
                    case error_type::OS_EINVAL:
                        *_error = OS_ERROR_BUILD("The value specified by abstime is invalid.", error_type::OS_EINVAL);
                        OS_ERROR_PTR_SET_POSITION(*_error);
                        break;
                    case error_type::OS_EPERM:
                        *_error = OS_ERROR_BUILD("The mutex was not owned by the current thread at the time of the call.", error_type::OS_EPERM);
                        OS_ERROR_PTR_SET_POSITION(*_error);
                        break;
                    default:
                        *_error = OS_ERROR_BUILD("Unmanaged error", error);
                        OS_ERROR_PTR_SET_POSITION(*_error);
                        break;
                    }
                }
                goto timeout;
            }
        }
        else
        {
            error = pthread_cond_wait (&sb.cond, &sb.mutex);
            if (error)
            {
                if(_error)
                {
                    switch (error_type(error))
                    {
                    case error_type::OS_ETIMEDOUT:
                        *_error = OS_ERROR_BUILD("The time specified by abstime to pthread_cond_timedwait() has passed.", error_type::OS_ETIMEDOUT);
                        OS_ERROR_PTR_SET_POSITION(*_error);
                        break;
                    case error_type::OS_EINVAL:
                        *_error = OS_ERROR_BUILD("The value specified by abstime is invalid.", error_type::OS_EINVAL);
                        OS_ERROR_PTR_SET_POSITION(*_error);
                        break;
                    default:
                        *_error = OS_ERROR_BUILD("Unmanaged error", error);
                        OS_ERROR_PTR_SET_POSITION(*_error);
                        break;
                    }
                }
                goto timeout;
            }
        }
    }

    if(sb.count == 0)
    {
        return false;
    }

    if(sb.r < sb.w && sb.end == 0)
    {
        //space available
        size_t data_available = sb.w - sb.r;
        if(data_available == 0 && sb.count)
        {
            data_available = sb.count;
        }

        if(size <= data_available)
        {
            memcpy(data, sb.buffer + sb.r, size);

            sb.r += size;

            sb.count -= size;

            already_received  = size;
        }
        else
        {
            memcpy(data, sb.buffer + sb.r, data_available);

            sb.r += data_available;

            sb.count -= data_available;

            size -= data_available;

            already_received  = data_available;
        }
    }
    else if(sb.r >= sb.w && sb.end > 0)
    {
        //rotation but not all data are read before end
        size_t data_available_between_r_and_size = sb.size - sb.r;

        if(data_available_between_r_and_size && size <= data_available_between_r_and_size)
        {
            memcpy(data + already_received, sb.buffer + sb.r, size);

            sb.r += size;

            sb.count -= size;

            already_received = size;

            data_available_between_r_and_size = size;

            size = 0;

        }
        else if(data_available_between_r_and_size && size > data_available_between_r_and_size)
        {
            memcpy(data + already_received, sb.buffer + sb.r, data_available_between_r_and_size);

            sb.r += data_available_between_r_and_size;

            sb.count -= data_available_between_r_and_size;

            already_received = data_available_between_r_and_size;

            size -= data_available_between_r_and_size;

            data_available_between_r_and_size = 0;

        }

        if(size && size <= sb.w)
        {
            memcpy(data + already_received, sb.buffer, size);

            sb.r = size;

            sb.count -= size;

            already_received += size;

            size = 0;

        }
        else if(size && size > sb.w)
        {
            memcpy(data + already_received, sb.buffer, sb.w);

            sb.r = sb.w;

            sb.count -= sb.w;

            already_received = sb.w;

            size -= sb.w;

        }
    }

    if(sb.count == 0)
    {
        sb.r = 0;
        sb.w = 0;
        sb.end = 0;
    }

timeout:
    pthread_mutex_unlock (&sb.mutex);
    pthread_cond_signal (&sb.cond);

    // return (error == 0);
    return error ? 0 : already_received;
}

size_t stream_buffer::receive_from_isr(uint8_t *data, size_t size, uint64_t time, error **error) OSAL_NOEXCEPT
{
    return receive(data, size, time, error);
}

void stream_buffer::reset() OSAL_NOEXCEPT
{
    sb.r = 0;
    sb.w = 0;
    sb.end = 0;
    memset(sb.buffer, '\0', sb.size);
}

bool stream_buffer::is_empty() const OSAL_NOEXCEPT
{
    return sb.count == 0;
}

bool stream_buffer::is_full() const OSAL_NOEXCEPT
{
    return (sb.size - sb.count) == 0;
}

size_t stream_buffer::size() const OSAL_NOEXCEPT
{
    return sb.count;
}

size_t stream_buffer::bytes_free() const
{
    return sb.size - sb.count;
}

}
}
