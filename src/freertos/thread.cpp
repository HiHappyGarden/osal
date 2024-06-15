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

#include <FreeRTOS.h>
#include <task.h>

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
    wrapper->fn(wrapper->arg);
}


thread::thread(const char *name, uint32_t priority, size_t stack_size, thread::handler handler) OSAL_NOEXCEPT
    : priority(priority)
    , stack_size(stack_size)
    , h(handler)
{
    if(name)
    {
        strncpy(this->name, name, sizeof(this->name) - 1);
    }
    else
    {
        memset(this->name, '\0', sizeof(this->name));
    }
    t.args_wrp.fn = handler;
}



osal::exit thread::create(void* arg, class error** error) OSAL_NOEXCEPT
{
    if(t.args_wrp.fn == nullptr)
    {
        return exit::KO;
    }

    t.args_wrp.arg = arg;

    configSTACK_DEPTH_TYPE stack_depth =  stack_size / sizeof (configSTACK_DEPTH_TYPE);
    if (xTaskCreate (&thread_data::args_wrapper::wrap_func, name, stack_depth, &t.args_wrp, priority, &t.handler) == pdPASS)
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

osal::exit thread::exit() OSAL_NOEXCEPT
{
    if(t.handler)
    {
        vTaskDelete(t.handler);
        t.handler = nullptr;
        return exit::OK;
    }
    return exit::KO;
}

osal::exit thread::join(error** error) const OSAL_NOEXCEPT
{
    return exit::OK;
}

}
}
