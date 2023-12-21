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
    : buffer_size(size * message_size)
{


}

queue::~queue() OS_NOEXCEPT
{

}

osal::exit queue::fetch(void* msg, uint64_t time, error** _error) OS_NOEXCEPT
{
    return exit::OK;
}

inline exit queue::fetch_from_isr(void* msg, uint64_t time, error** error) OS_NOEXCEPT
{
    return fetch(msg, time, error);
}

osal::exit queue::post(const uint8_t* msg, uint64_t time, error** _error) OS_NOEXCEPT
{
   return exit::OK;;
}

inline exit queue::post_from_isr(const uint8_t* msg, uint64_t time, error** error) OS_NOEXCEPT
{
    return post(msg, time, error);
}

size_t queue::size() const OS_NOEXCEPT
{
    return 0;
}

}
}
