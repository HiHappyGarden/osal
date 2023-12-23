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
#include "osal/queue.hpp"

namespace osal
{
inline namespace v1
{

queue::queue(size_t size, size_t message_size, error** error) OS_NOEXCEPT
    : q {
       .size = size,
       .count = 0,
       .handle = xQueueCreate(size, message_size)
    }
{
    if(q.handle == nullptr && error)
    {
        *error = OS_ERROR_BUILD("xQueueCreate() fail.", error_type::OS_EFAULT);
        OS_ERROR_PTR_SET_POSITION(*error);
    }
}

queue::~queue() OS_NOEXCEPT
{
    if(q.handle)
    {
        vQueueDelete(q.handle);
        q.handle = nullptr;
    }
}

osal::exit queue::fetch(void* msg, uint64_t time, error** error) OS_NOEXCEPT
{
    if(q.handle == nullptr)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("xEventGroupCreate() fail.", error_type::OS_EFAULT);
            OS_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }

    if(xQueueReceive(q.handle, msg, tmo_to_ticks(time)) == pdTRUE && q.count)
    {
        q.count--;
        return exit::OK;
    }

    return exit::KO;
}

inline exit queue::fetch_from_isr(void* msg, uint64_t, error** error) OS_NOEXCEPT
{
    if(q.handle == nullptr)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("xEventGroupCreate() fail.", error_type::OS_EFAULT);
            OS_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }

    BaseType_t success = xQueueReceiveFromISR( q.handle, msg, nullptr);
    portYIELD_FROM_ISR(pdFALSE);
    if(success == pdTRUE && q.count)
    {
        q.count--;
        return exit::OK;
    }

    return exit::KO;
}

osal::exit queue::post(const uint8_t* msg, uint64_t time, error** error) OS_NOEXCEPT
{
    if(q.handle == nullptr)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("xEventGroupCreate() fail.", error_type::OS_EFAULT);
            OS_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }

    if(xQueueSendToBack(q.handle, msg, tmo_to_ticks(time)) == pdTRUE)
    {
        q.count++;
        return exit::OK;
    }

    return exit::KO;
}

inline exit queue::post_from_isr(const uint8_t* msg, uint64_t, error** error) OS_NOEXCEPT
{
    if(q.handle == nullptr)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("xEventGroupCreate() fail.", error_type::OS_EFAULT);
            OS_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }
    
    BaseType_t success = xQueueSendToBackFromISR(q.handle, msg, nullptr);
    portYIELD_FROM_ISR(pdFALSE);

    if(success == pdTRUE)
    {
        q.count++;
        return exit::OK;
    }

    return exit::KO;
}

size_t queue::size() const OS_NOEXCEPT
{
    return q.size;
}

}
}
