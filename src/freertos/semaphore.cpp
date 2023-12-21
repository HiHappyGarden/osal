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
#include "osal/semaphore.hpp"


namespace osal
{
inline namespace v1
{

semaphore::semaphore(size_t count) OS_NOEXCEPT
{

}

semaphore::~semaphore()
{

}

osal::exit semaphore::wait(uint64_t time, error** _error) OS_NOEXCEPT
{

    return exit::OK;
}

osal::exit semaphore::wait_from_isr(uint64_t time, error **error) OS_NOEXCEPT
{
    return wait(time, error);
}

void semaphore::signal()
{

}

void semaphore::signal_from_isr() OS_NOEXCEPT
{
    signal();
}


}
}
