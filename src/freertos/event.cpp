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
#include "osal/event.hpp"
#include "osal/osal.hpp"

namespace osal
{
inline namespace v1
{

event::event() OS_NOEXCEPT : e{ .handle = xEventGroupCreate() } { }

event::~event() OS_NOEXCEPT
{
    if(e.handle)
    {
        vEventGroupDelete(e.handle);
        e.handle = nullptr;
    }
}


osal::exit event::wait(uint32_t mask, uint32_t& value, uint64_t time, error** _error) OS_NOEXCEPT
{
    if(e.handle == nullptr)
    {
        if(_error)
        {
            *_error = OS_ERROR_BUILD("xEventGroupCreate() fail.", error_type::OS_EFAULT);
            OS_ERROR_PTR_SET_POSITION(*_error);
            return exit::KO;
        }
    }
    value = xEventGroupWaitBits(
            e.handle,
            mask,
            pdFALSE,
            pdFALSE,
            ms_to_us(time));

    value &= mask;
    return (value == 0) ? exit::OK : exit::KO;
}

osal::exit event::wait_from_isr(uint32_t mask, uint32_t& value, uint64_t time, error **error)
{
    return wait(mask, value, time, error);
}

void event::set(uint32_t value)
{
    if(e.handle)
    {
        xEventGroupSetBits(e.handle, value);
    }
}

void event::set_from_isr(uint32_t value)
{
    if(e.handle)
    {
        xEventGroupSetBitsFromISR(e.handle, value, NULL);
    }
}

uint32_t event::get() OS_NOEXCEPT
{
    if(e.handle)
    {
        return xEventGroupGetBits(e.handle);
    }
    return 0;
}

uint32_t event::get_from_isr() OS_NOEXCEPT
{
    if(e.handle)
    {
        EventBits_t ret = xEventGroupGetBitsFromISR(e.handle);
        portYIELD_FROM_ISR(pdFALSE);
        return ret;
    }
    return 0;
}

void event::clear(uint32_t value)
{
    if(e.handle)
    {
        xEventGroupClearBits(e.handle, value);
    }
}

inline void event::clear_from_isr(uint32_t value)
{
    if(e.handle)
    {
        xEventGroupClearBitsFromISR(e.handle, value);
        portYIELD_FROM_ISR(pdFALSE);
    }
}

}
}
