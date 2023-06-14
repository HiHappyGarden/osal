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

namespace osal
{
inline namespace v1
{

template<size_t size_name>
class thread_private final : public thread
{
    pthread_t thread = {0};

    char name[size_name];
    uint32_t priority;
    size_t stack_size;
    handler h;

public:

    thread_private(const char *name, uint32_t priority, size_t stack_size, handler handler) OS_NOEXCEPT;


    bool create (void* arg) OS_NOEXCEPT override;

};

template<size_t size_name>
thread_private<size_name>::thread_private(const char *name, uint32_t priority, size_t stack_size, thread::handler handler) OS_NOEXCEPT
    : priority(priority)
    , stack_size(stack_size)
{
    if(name)
    {
        strncpy(this->name, name, sizeof(this->name));
    }
}

template<size_t size_name>
bool thread_private<size_name>::create(void *arg) noexcept
{

    int result;

    pthread_attr_t attr;

    pthread_attr_init (&attr);
//    pthread_attr_setstacksize (&attr, PTHREAD_STACK_MIN + stack_size);

//    //#if defined(USE_SCHED_FIFO)
//    CC_STATIC_ASSERT (_POSIX_THREAD_PRIORITY_SCHEDULING > 0);
//    struct sched_param param = {.sched_priority = priority};
//    pthread_attr_setinheritsched (&attr, PTHREAD_EXPLICIT_SCHED);
//    pthread_attr_setschedpolicy (&attr, SCHED_FIFO);
//    pthread_attr_setschedparam (&attr, &param);
//    //#endif

//    result = pthread_create (thread, &attr, (void *)entry, arg);
//    if (result != 0)
//    {
//        free (thread);
//    }

//    pthread_setname_np (*thread, name);
    return result == 0;
}

osal::thread* thread::build(const char *name, uint32_t priority, size_t stack_size, thread::handler handler) OS_NOEXCEPT
{
    if(strlen(name) >= 30 )
    {
        return nullptr;
    }
    thread* t = dynamic_cast<thread*>(new thread_private<30>(name, priority, stack_size, handler));
    if(t == nullptr)
    {
        return nullptr;
    }
    return t;
}

}
}
