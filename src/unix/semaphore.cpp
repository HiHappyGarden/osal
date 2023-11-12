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
#include "osal/semaphore.hpp"

#include <pthread.h>
#include <errno.h>

namespace osal
{
inline namespace v1
{

semaphore::semaphore(size_t count) OS_NOEXCEPT
{

    pthread_mutexattr_t mattr{0};
    pthread_condattr_t cattr{0};

    pthread_condattr_init (&cattr);
    pthread_condattr_setclock (&cattr, CLOCK_MONOTONIC);
    pthread_cond_init (&sem.cond, &cattr);
    pthread_mutexattr_init (&mattr);
    pthread_mutexattr_setprotocol (&mattr, PTHREAD_PRIO_INHERIT);
    pthread_mutex_init (&sem.mutex, &mattr);
    sem.count = count;
}

semaphore::~semaphore()
{
    pthread_cond_destroy (&sem.cond);
    pthread_mutex_destroy (&sem.mutex);
}

osal::exit semaphore::wait(uint64_t time, error** _error) OS_NOEXCEPT
{
    timespec ts{0};
    uint8_t error     = 0;
    uint64_t nsec = (uint64_t)time * 1'000'000;


    clock_gettime (CLOCK_MONOTONIC, &ts);
    nsec += ts.tv_nsec;
    if (nsec > NSECS_PER_SEC)
    {
        ts.tv_sec += nsec / NSECS_PER_SEC;
        nsec %= NSECS_PER_SEC;
    }
    ts.tv_nsec = nsec;

    pthread_mutex_lock (&sem.mutex);
    while (sem.count == 0)
    {
        if (time != WAIT_FOREVER)
        {
            error = pthread_cond_timedwait (&sem.cond, &sem.mutex, &ts);
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
            error = pthread_cond_wait (&sem.cond, &sem.mutex);
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

    sem.count--;

timeout:
    pthread_mutex_unlock (&sem.mutex);
    return (error == 0) ? exit::OK : exit::KO;
}

osal::exit semaphore::wait_from_isr(uint64_t time, error **error) OS_NOEXCEPT
{
    return wait(time, error);
}

void semaphore::signal()
{
    pthread_mutex_lock (&sem.mutex);
    sem.count++;
    pthread_mutex_unlock (&sem.mutex);
    pthread_cond_signal (&sem.cond);
}

void semaphore::signal_from_isr() OS_NOEXCEPT
{
    signal();
}


}
}
