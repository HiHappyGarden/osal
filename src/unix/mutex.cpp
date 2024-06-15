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
#include "osal/mutex.hpp"
#include <pthread.h>
#include <stdint.h>

namespace osal
{
inline namespace v1
{


mutex::mutex(class error** error) OSAL_NOEXCEPT
{
    pthread_mutexattr_t mattr{0};

    pthread_mutexattr_init (&mattr);
    pthread_mutexattr_setprotocol (&mattr, PTHREAD_PRIO_INHERIT);
    pthread_mutexattr_settype (&mattr, PTHREAD_MUTEX_RECURSIVE);

    int32_t result = pthread_mutex_init (&m, &mattr);
    if (result && error)
    {
        switch (error_type(result)) {
        case error_type::OS_EAGAIN:
            *error = OSAL_ERROR_BUILD("The system lacked the necessary resources (other than memory) to initialise another mutex.", error_type::OS_EAGAIN);
            OSAL_ERROR_PTR_SET_POSITION(*error);
            break;
        case error_type::OS_ENOMEM:
            *error = OSAL_ERROR_BUILD("Insufficient memory exists to initialise the mutex.", error_type::OS_ENOMEM);
            OSAL_ERROR_PTR_SET_POSITION(*error);
            break;
        case error_type::OS_EPERM:
            *error = OSAL_ERROR_BUILD("The caller does not have the privilege to perform the operation.", error_type::OS_EPERM);
            OSAL_ERROR_PTR_SET_POSITION(*error);
            break;
        case error_type::OS_EBUSY:
            *error = OSAL_ERROR_BUILD("The caller does not have the privilege to perform the operation.", error_type::OS_EBUSY);
            OSAL_ERROR_PTR_SET_POSITION(*error);
            break;
        case error_type::OS_EINVAL:
            *error = OSAL_ERROR_BUILD("The caller does not have the privilege to perform the operation.", error_type::OS_EINVAL);
            OSAL_ERROR_PTR_SET_POSITION(*error);
            break;
        default:
            *error = OSAL_ERROR_BUILD("Unmanaged error", result);
            OSAL_ERROR_PTR_SET_POSITION(*error);
            break;
        }
    }
}

mutex::~mutex() OSAL_NOEXCEPT
{
    pthread_mutex_destroy (&m);
    memset(&m, 0, sizeof(mutex));
}

void mutex::lock() OSAL_NOEXCEPT
{
    pthread_mutex_lock (&m);
}

inline void mutex::lock_from_isr() OSAL_NOEXCEPT
{
    lock();
}

void mutex::unlock() OSAL_NOEXCEPT
{
    pthread_mutex_unlock (&m);
}

inline void mutex::unlock_from_isr() OSAL_NOEXCEPT
{
    unlock();
}

}
}
