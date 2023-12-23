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


namespace osal
{
inline namespace v1
{


void thread_data::args_wrapper::wrap_func(void* arg)
{
    if(arg == nullptr)
    {
        return;
    }

    auto wrapper = static_cast<thread_data::args_wrapper*>(arg);
    wrapper->handler(wrapper->arg);
}


thread::thread(const char *name, uint32_t priority, size_t stack_size, thread::handler handler) OS_NOEXCEPT
    : priority(priority)
    , stack_size(stack_size)
    , h(handler)
{
    if(name)
    {
        strncpy(this->name, name, sizeof(name) - 1);
    }
    else
    {
        memset(this->name, '\0', sizeof(name));
    }
    t.arg.handler = handler;
}



osal::exit thread::create(void* arg, class error** error) OS_NOEXCEPT
{


    t.arg.arg = arg;

    configSTACK_DEPTH_TYPE stack_depth =  stack_size / sizeof (configSTACK_DEPTH_TYPE);
    if (xTaskCreate (&thread_data::args_wrapper::wrap_func, name, stack_depth, &t.arg, priority, &t.handle) == pdPASS)
    {
        return exit::OK;
    }

    if(error)
    {
        *error = OS_ERROR_BUILD("xTaskCreate() fail.", error_type::OS_EFAULT);
        OS_ERROR_PTR_SET_POSITION(*error);
    }
    return exit::KO;
}

osal::exit thread::exit() OS_NOEXCEPT
{
    if(t.handle)
    {
        vTaskDelete(t.handle);
        t.handle= NULL;
        return exit::OK;
    }
    return exit::KO;
}

osal::exit thread::join(error** error) const OS_NOEXCEPT
{
    return exit::OK;
}

}
}
