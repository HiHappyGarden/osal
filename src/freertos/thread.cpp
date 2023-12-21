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


thread::thread(const char *name, uint32_t priority, size_t stack_size, thread::handler handler) OS_NOEXCEPT
    : priority(priority)
    , stack_size(stack_size)
    , h(handler)
{
    if(name)
    {
        strncpy(this->name, name, sizeof(name) - 1);
    }
}


osal::exit thread::create(void* arg, class error** error) OS_NOEXCEPT
{

    return exit::OK;
}

osal::exit thread::exit() OS_NOEXCEPT
{
    return exit::OK;
}

osal::exit thread::join(error** error) const OS_NOEXCEPT
{

    return exit::OK;
}



}
}
