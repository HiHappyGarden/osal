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

namespace osal
{
inline namespace v1
{

event::event() OS_NOEXCEPT
{

}

event::~event() OS_NOEXCEPT
{

}

osal::exit event::wait(uint32_t mask, uint32_t *value, uint64_t time, error** _error) OS_NOEXCEPT
{

    return exit::OK;
}

inline osal::exit event::wait_from_isr(uint32_t mask, uint32_t *value, uint64_t time, error **error)
{
    return wait(mask, value, time, error);
}

void event::set(uint32_t value)
{

}

inline void event::set_from_isr(uint32_t value)
{
    return set(value);
}

uint32_t event::get() OS_NOEXCEPT
{
    uint32_t ret = 0;

    return ret;
}

uint32_t event::get_from_isr() OS_NOEXCEPT
{
    return get();
}

void event::clear(uint32_t value)
{

}

inline void event::clear_from_isr(uint32_t value)
{
    clear(value);
}

}
}
