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
#include "osal/thread.hpp"

#include <pthread.h>
#include <string.h>
#include <limits.h>

namespace osal
{
inline namespace v1
{

class thread_private final : public thread
{
    pthread_t t = {0};
    char name[33];
    uint32_t priority;
    size_t stack_size;
    thread::handler handler;

public:

    thread_private(const char *name, uint32_t priority, size_t stack_size, thread::handler handler) OS_NOEXCEPT;


    bool create (void* arg) OS_NOEXCEPT override;

    bool exit() OS_NOEXCEPT override;

};

thread_private::thread_private(const char *name, uint32_t priority, size_t stack_size, thread::handler handler) OS_NOEXCEPT
    : priority(priority)
    , stack_size(stack_size)
    , handler(handler)
{
    if(name)
    {
        strncpy(this->name, name, sizeof(name) - 1);
    }
}

#ifdef __MACH__
bool thread_private::create(void* _Nullable arg = nullptr) OS_NOEXCEPT
#else
bool thread_private::create(void* arg = nullptr) OS_NOEXCEPT
#endif

{

    int result;

    pthread_attr_t attr;

    pthread_attr_init (&attr);
    pthread_attr_setstacksize (&attr, PTHREAD_STACK_MIN + stack_size);

#if defined(USE_SCHED_FIFO)
//    CC_STATIC_ASSERT (_POSIX_THREAD_PRIORITY_SCHEDULING > 0);
    struct sched_param param = {.sched_priority = static_cast<int>(priority)};
    pthread_attr_setinheritsched (&attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy (&attr, SCHED_FIFO);
    pthread_attr_setschedparam (&attr, &param);
#else
    priority = 0;
#endif



    result = pthread_create (&t, &attr, handler, arg);

#ifndef __MACH__
    pthread_setname_np (t, name);
#endif
    return result == 0;
}

bool thread_private::exit() noexcept
{

    pthread_exit(&t);
    return true;
}

#ifdef __MACH__
thread* _Nullable thread::build(const char* _Nullable name, uint32_t priority, size_t stack_size, handler handler) OS_NOEXCEPT
#else
thread* thread::build(const char* name, uint32_t priority, size_t stack_size, handler handler) OS_NOEXCEPT
#endif

{
    if(name == nullptr || handler == nullptr)
    {
        return nullptr;
    }

    thread* t = dynamic_cast<thread*>(new thread_private(name, priority, stack_size, handler));
    if(t == nullptr)
    {
        return nullptr;
    }
    return t;
}


}
}
