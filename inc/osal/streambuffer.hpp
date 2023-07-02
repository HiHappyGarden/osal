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
#pragma once


#include "error.hpp"
#include "osal_sys/osal_sys.hpp"

#include <stdlib.h>

namespace osal
{
inline namespace v1
{
class stream_buffer
{
    stream_buffer (size_t size, size_t trigger_size, error** _error = nullptr) OS_NOEXCEPT;


    /**
     * @brief Deleted copy constructor.
     */
    stream_buffer(const stream_buffer&) = delete;

    /**
     * @brief Deleted copy assignment operator.
     */
    stream_buffer& operator=(const stream_buffer&) = delete;

    /**
     * @brief Deleted move constructor.
     */
    stream_buffer(stream_buffer&&) = delete;

    /**
     * @brief Deleted move assignment operator.
     */
    stream_buffer& operator=(stream_buffer&&) = delete;

    /**
     * @brief Virtual destructor.
     */
    ~stream_buffer() OS_NOEXCEPT;

    size_t send(const uint8_t* data, size_t size, uint64_t time, error** error = nullptr)  OS_NOEXCEPT;

    size_t send_from_isr(const uint8_t* data, size_t size, uint64_t time, error** error = nullptr) OS_NOEXCEPT;

    size_t receive(uint8_t* data, size_t size, uint64_t time, error** error = nullptr)  OS_NOEXCEPT;

    size_t receive_from_isr(uint8_t* data, size_t size, uint64_t time, error** error = nullptr) OS_NOEXCEPT;

    bool reset () OS_NOEXCEPT;

    bool is_empty() OS_NOEXCEPT;

    bool is_full() OS_NOEXCEPT;

    size_t size() OS_NOEXCEPT;

    size_t bytes_available();





private:
    stream_buffer_data sb;
};
}
}


