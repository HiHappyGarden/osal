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
#include "osal/thread.hpp"

#include <pthread.h>
#include <string.h>
#include <limits.h>

namespace osal
{
inline namespace v1
{


thread::thread(const char *name, uint32_t priority, size_t stack_size, thread::handler handler) OSAL_NOEXCEPT
    : priority(priority)
    , stack_size(stack_size)
    , h(handler)
{
    if(name)
    {
        strncpy(this->name, name, sizeof(name) - 1);
    }
}

osal::exit thread::create(void* arg, class error** error) OSAL_NOEXCEPT
{
    pthread_attr_t attr{0};

    pthread_attr_init (&attr);
    pthread_attr_setstacksize (&attr, PTHREAD_STACK_MIN + stack_size);

#if defined(OS_USE_SCHED_FIFO)
    sched_param param = {.sched_priority = static_cast<int>(priority)};
    pthread_attr_setinheritsched (&attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy (&attr, SCHED_FIFO);
    pthread_attr_setschedparam (&attr, &param);
#else
    priority = 0;
#endif

    int32_t result = pthread_create (&t, &attr, h, arg);
    if(result && error)
    {
        switch (error_type(result))
        {
        case error_type::OS_EAGAIN:
            *error = OSAL_ERROR_BUILD("Insufficient resources to create another thread.", error_type::OS_EAGAIN);
            OSAL_ERROR_PTR_SET_POSITION(*error);
            break;
        case error_type::OS_EINVAL:
            *error = OSAL_ERROR_BUILD("Invalid settings in attr.", error_type::OS_EINVAL);
            OSAL_ERROR_PTR_SET_POSITION(*error);
            break;
        case error_type::OS_EPERM:
            *error = OSAL_ERROR_BUILD("No permission to set the scheduling policy and parameters specified in attr.", error_type::OS_EINVAL);
            OSAL_ERROR_PTR_SET_POSITION(*error);
            break;
        default:
            *error = OSAL_ERROR_BUILD("Unmanaged error", result);
            OSAL_ERROR_PTR_SET_POSITION(*error);
            break;
        }
    }


    if(strlen(name))
    {
        pthread_setname_np (t, name);
    }
    return (result == 0) ? exit::OK : exit::KO;
}

osal::exit thread::exit() OSAL_NOEXCEPT
{
    pthread_exit(&t);
    return exit::OK;
}

osal::exit thread::join(error** error) const OSAL_NOEXCEPT
{
    uint32_t result = pthread_join(t, nullptr);
    if(result && error)
    {
        switch (error_type(result))
        {
        case error_type::OS_EDEADLK:
            *error = OSAL_ERROR_BUILD("A deadlock was detected or thread specifies the calling thread.", error_type::OS_EDEADLK);
            OSAL_ERROR_PTR_SET_POSITION(*error);
            break;
        case error_type::OS_EINVAL:
            *error = OSAL_ERROR_BUILD("Another thread is already waiting to join with this thread.", error_type::OS_EINVAL);
            OSAL_ERROR_PTR_SET_POSITION(*error);
            break;
        case error_type::OS_ESRCH:
            *error = OSAL_ERROR_BUILD("No thread with the ID thread could be found.", error_type::OS_ESRCH);
            OSAL_ERROR_PTR_SET_POSITION(*error);
            break;
        default:
            *error = OSAL_ERROR_BUILD("Unmanaged error", result);
            OSAL_ERROR_PTR_SET_POSITION(*error);
            break;
        }
    }

    return (result == 0) ? exit::OK : exit::KO;
}



}
}
