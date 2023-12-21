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
#include "osal/mutex.hpp"


namespace osal
{
inline namespace v1
{


mutex::mutex(class error** error) OS_NOEXCEPT
{

}

mutex::~mutex() OS_NOEXCEPT
{

}

void mutex::lock() OS_NOEXCEPT
{

}

inline void mutex::lock_from_isr() OS_NOEXCEPT
{
    lock();
}

void mutex::unlock() OS_NOEXCEPT
{

}

inline void mutex::unlock_from_isr() OS_NOEXCEPT
{
    unlock();
}

}
}
