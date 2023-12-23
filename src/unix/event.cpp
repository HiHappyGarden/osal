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
#include "osal/event.hpp"
#include <pthread.h>

namespace osal
{
inline namespace v1
{

event::event(error**) OS_NOEXCEPT
{
    pthread_mutexattr_t mattr{0};
    pthread_condattr_t cattr{0};

    pthread_condattr_init (&cattr);
    pthread_condattr_setclock (&cattr, CLOCK_MONOTONIC);
    pthread_cond_init (&e.cond, &cattr);
    pthread_mutexattr_init (&mattr);
    pthread_mutexattr_setprotocol (&mattr, PTHREAD_PRIO_INHERIT);
    pthread_mutex_init (&e.mutex, &mattr);
    e.flags = 0;
}

event::~event() OS_NOEXCEPT
{
    pthread_cond_destroy (&e.cond);
    pthread_mutex_destroy (&e.mutex);
}

osal::exit event::wait(uint32_t mask, uint32_t& value, uint64_t time, error** _error) OS_NOEXCEPT
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

    pthread_mutex_lock (&e.mutex);

    while ((e.flags & mask) == 0)
    {
        if (time != WAIT_FOREVER)
        {
            error = pthread_cond_timedwait (&e.cond, &e.mutex, &ts);
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
            error = pthread_cond_wait (&e.cond, &e.mutex);
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

timeout:
    value = e.flags & mask;
    pthread_mutex_unlock (&e.mutex);
    return (error == 0) ? exit::OK : exit::KO;
}

inline osal::exit event::wait_from_isr(uint32_t mask, uint32_t& value, uint64_t time, error **error)
{
    return wait(mask, value, time, error);
}

void event::set(uint32_t value)
{
    pthread_mutex_lock (&e.mutex);
    e.flags |= value;
    pthread_mutex_unlock (&e.mutex);
    pthread_cond_signal (&e.cond);
}

inline void event::set_from_isr(uint32_t value)
{
    return set(value);
}

uint32_t event::get() OS_NOEXCEPT
{
    uint32_t ret = 0;
    pthread_mutex_lock (&e.mutex);
    ret = e.flags;
    pthread_mutex_unlock (&e.mutex);
    pthread_cond_signal (&e.cond);
    return ret;
}

inline uint32_t event::get_from_isr() OS_NOEXCEPT
{
    return get();
}

void event::clear(uint32_t value)
{
    pthread_mutex_lock (&e.mutex);
    e.flags &= ~value;
    pthread_mutex_unlock (&e.mutex);
    pthread_cond_signal (&e.cond);
}

inline void event::clear_from_isr(uint32_t value)
{
    clear(value);
}

}
}
