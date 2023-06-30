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

class mutex_private final : public mutex
{
    pthread_mutex_t mutex = {0};
public:
    mutex_private(class error** error) OS_NOEXCEPT;

    ~mutex_private() OS_NOEXCEPT;

    void lock () OS_NOEXCEPT override;

    void unlock () OS_NOEXCEPT override;


};

mutex_private::mutex_private(class error** error) OS_NOEXCEPT
{
    int32_t result;
    pthread_mutexattr_t mattr;

//   CC_STATIC_ASSERT (_POSIX_THREAD_PRIO_INHERIT > 0);
    pthread_mutexattr_init (&mattr);
    pthread_mutexattr_setprotocol (&mattr, PTHREAD_PRIO_INHERIT);
    pthread_mutexattr_settype (&mattr, PTHREAD_MUTEX_RECURSIVE);

    result = pthread_mutex_init (&mutex, &mattr);
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
            break;
        }

    }
}

inline mutex_private::~mutex_private() OS_NOEXCEPT
{
    pthread_mutex_destroy (&mutex);
    memset(&mutex, 0, sizeof(mutex));
}

inline void mutex_private::lock() OS_NOEXCEPT
{
    pthread_mutex_lock (&mutex);
}

inline void mutex_private::unlock() OS_NOEXCEPT
{
    pthread_mutex_unlock (&mutex);
}


#ifdef __MACH__
static inline mutex* _Nullable build() OS_NOEXCEPT
#else
static inline mutex* build(class error** error) OS_NOEXCEPT
#endif
{
    mutex* m = dynamic_cast<mutex*>(new mutex_private(error));
    if(m == nullptr)
    {
        return nullptr;
    }
    return m;
}



}
}
