/***************************************************************************
 *
 * OSAL
 * Copyright (C) 2023  Antonio Salsi <passy.linux@zresa.it>
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
#include "osal/queue.hpp"

namespace osal
{
inline namespace v1
{

queue::queue(size_t size, size_t message_size, error** error) OS_NOEXCEPT
    : buffer_size(size * message_size)
{
    // kiwi_os_mbox_t * mbox;
    pthread_mutexattr_t mattr{0};
    pthread_condattr_t cattr{0};

    q.msg = new uint8_t[buffer_size];
    if (q.msg == nullptr)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("Out off memory.", error_type::OS_ENOMEM);
            OS_ERROR_PTR_SET_POSITION(*error);
        }
        return;
    }
    memset(q.msg, 0, buffer_size);

    pthread_condattr_init (&cattr);
    pthread_condattr_setclock (&cattr, CLOCK_MONOTONIC);
    pthread_cond_init (&q.cond, &cattr);
    pthread_mutexattr_init (&mattr);
    pthread_mutexattr_setprotocol (&mattr, PTHREAD_PRIO_INHERIT);
    pthread_mutex_init (&q.mutex, &mattr);

    q.r     = 0;
    q.w     = 0;
    q.count = 0;
    q.size  = size;
    q.message_size  = message_size;

}

queue::~queue() OS_NOEXCEPT
{
    pthread_cond_destroy (&q.cond);
    pthread_mutex_destroy (&q.mutex);

    if(q.msg)
    {
        for(size_t i = 0; i < buffer_size; i++)
        {
            q.msg[i] = 0;
        }
        delete[] q.msg;
        q.msg = nullptr;
    }
}

bool queue::fetch(void *msg, uint64_t time, error** _error) OS_NOEXCEPT
{
    struct timespec ts{0};
    uint8_t error     = 0;
    uint64_t nsec = (uint64_t)time * 1'000'000;

    if(msg == nullptr)
    {
        if(error)
        {
            *_error = OS_ERROR_BUILD("Invalid argument.", error_type::OS_EINVAL);
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

    pthread_mutex_lock (&q.mutex);

    while (q.count == 0)
    {
        if (time != WAIT_FOREVER)
        {
            error = pthread_cond_timedwait (&q.cond, &q.mutex, &ts);
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
            error = pthread_cond_wait (&q.cond, &q.mutex);
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

    memset(msg, 0, q.message_size);
    if(q.count == 0)
    {
        if(error)
        {
            *_error = OS_ERROR_BUILD("Message size 0.", error_type::OS_EUCLEAN);
            OS_ERROR_PTR_SET_POSITION(*_error);
        }
        return false;
    }

    memcpy(msg, q.msg + ( q.r * q.message_size), q.message_size);

    q.r++;

    if (q.r == q.size)
        q.r = 0;

    q.count--;

timeout:
    pthread_mutex_unlock (&q.mutex);
    pthread_cond_signal (&q.cond);

    return (error == 0);
}

inline bool queue::fetch_from_isr(void *msg, uint64_t time, error** error) OS_NOEXCEPT
{
    return fetch(msg, time, error);
}

bool queue::post(const uint8_t *msg, uint64_t time, error** _error) OS_NOEXCEPT
{
    struct timespec ts{0};
    uint8_t error     = 0;
    uint64_t nsec = (uint64_t)time * 1000 * 1000;


    if (time != WAIT_FOREVER)
    {
        clock_gettime (CLOCK_MONOTONIC, &ts);
        nsec += ts.tv_nsec;

        ts.tv_sec += nsec / NSECS_PER_SEC;
        ts.tv_nsec = nsec % NSECS_PER_SEC;
    }

    pthread_mutex_lock (&q.mutex);

    while (q.count == q.size)
    {
        if (time != WAIT_FOREVER)
        {
            error = pthread_cond_timedwait (&q.cond, &q.mutex, &ts);
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
            error = pthread_cond_wait (&q.cond, &q.mutex);
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

    memcpy(q.msg + (q.w * q.message_size), msg, q.message_size);

    q.w++;

    if (q.w == q.size)
        q.w = 0;

    q.count++;

timeout:
    pthread_mutex_unlock (&q.mutex);
    pthread_cond_signal (&q.cond);

    return (error == 0);

}

inline bool queue::post_from_isr(const uint8_t *msg, uint64_t time, error** error) OS_NOEXCEPT
{
    return post(msg, time, error);
}

size_t queue::size() const OS_NOEXCEPT
{

    return q.count;
}

}
}
