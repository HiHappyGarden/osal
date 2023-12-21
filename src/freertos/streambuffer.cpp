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
#include "osal/streambuffer.hpp"

namespace osal
{
inline namespace v1
{

stream_buffer::stream_buffer(size_t size, size_t trigger_size, error** error) OS_NOEXCEPT
{

}

stream_buffer::~stream_buffer()
{

}

size_t stream_buffer::send(const uint8_t *data, size_t size, uint64_t time, error** _error) OS_NOEXCEPT
{
    return 0;
}

inline size_t stream_buffer::send_from_isr(const uint8_t *data, size_t size, uint64_t time, error **error) OS_NOEXCEPT
{
    return send(data, size, time, error);
}

size_t stream_buffer::receive(uint8_t *data, size_t size, uint64_t time, error **_error) OS_NOEXCEPT
{
    return 0;
}

size_t stream_buffer::receive_from_isr(uint8_t *data, size_t size, uint64_t time, error **error) OS_NOEXCEPT
{
    return receive(data, size, time, error);
}

void stream_buffer::reset() OS_NOEXCEPT
{

}

bool stream_buffer::is_empty() const OS_NOEXCEPT
{
    return true;
}

bool stream_buffer::is_full() const OS_NOEXCEPT
{
    return false;
}

size_t stream_buffer::size() const OS_NOEXCEPT
{
    return 0;
}

size_t stream_buffer::bytes_free() const
{
    return 0;
}

}
}
