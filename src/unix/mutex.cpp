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
#include "osal/mutex.hpp"
#include <pthread.h>
#include <stdint.h>

namespace osal
{
inline namespace v1
{


mutex::mutex(class error** error) OS_NOEXCEPT
{
    int32_t result;
    pthread_mutexattr_t mattr;

    pthread_mutexattr_init (&mattr);
    pthread_mutexattr_setprotocol (&mattr, PTHREAD_PRIO_INHERIT);
    pthread_mutexattr_settype (&mattr, PTHREAD_MUTEX_RECURSIVE);

    result = pthread_mutex_init (&m, &mattr);
    if (result && error)
    {
        switch (error_type(result)) {
        case error_type::OS_EAGAIN:
                *error = OS_ERROR_BUILD("The system lacked the necessary resources (other than memory) to initialise another mutex.", error_type::OS_EAGAIN);
            break;
        case error_type::OS_ENOMEM:
                *error = OS_ERROR_BUILD("Insufficient memory exists to initialise the mutex.", error_type::OS_ENOMEM);
            break;
        case error_type::OS_EPERM:
                *error = OS_ERROR_BUILD("The caller does not have the privilege to perform the operation.", error_type::OS_EPERM);
            break;
        case error_type::OS_EBUSY:
                *error = OS_ERROR_BUILD("The caller does not have the privilege to perform the operation.", error_type::OS_EBUSY);
            break;
        case error_type::OS_EINVAL:
                *error = OS_ERROR_BUILD("The caller does not have the privilege to perform the operation.", error_type::OS_EINVAL);
            break;
        default:
            *error = OS_ERROR_BUILD("Unmanaged error", result);
            break;
        }
    }
}

mutex::~mutex() OS_NOEXCEPT
{
    pthread_mutex_destroy (&m);
    memset(&m, 0, sizeof(mutex));
}

void mutex::lock() OS_NOEXCEPT
{
    pthread_mutex_lock (&m);
}

void mutex::unlock() OS_NOEXCEPT
{
    pthread_mutex_unlock (&m);
}

}
}
