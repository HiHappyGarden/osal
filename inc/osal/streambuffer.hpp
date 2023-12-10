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

/**
* @brief Class for stream buffers.
*
* This class provides a stream buffer implementation.
* It is meant to be used for sending and receiving data in a stream-like manner.
*/
class stream_buffer
{
public:

    /**
     * @brief Constructor.
     *
     * @param size The total size (in bytes) of the stream buffer.
     * @param trigger_size The minimum number of bytes required to trigger sending or receiving.
     * @param _error Optional pointer to an error object to be populated in case of failure.
     */
    stream_buffer(size_t size, size_t trigger_size, error** _error = nullptr) OS_NOEXCEPT;

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

    /**
     * @brief Sends data to the stream buffer.
     *
     * This function sends the specified data to the stream buffer.
     * It blocks the caller until all the data is sent or until the specified time has elapsed.
     *
     * @param data Pointer to the data to be sent.
     * @param size The size (in bytes) of the data to be sent.
     * @param time The maximum time to wait for sending the data (in milliseconds).
     * @param error Optional pointer to an error object to be populated in case of failure.
     * @return The number of bytes sent.
     */
    size_t send(const uint8_t* data, size_t size, uint64_t time, error** error = nullptr) OS_NOEXCEPT;

    /**
     * @brief Sends data to the stream buffer from an ISR.
     *
     * This function is an ISR (Interrupt Service Routine) version of the send() function.
     * It has the same behavior as send(), but it is meant to be called from an ISR context.
     *
     * @param data Pointer to the data to be sent.
     * @param size The size (in bytes) of the data to be sent.
     * @param time The maximum time to wait for sending the data (in milliseconds).
     * @param error Optional pointer to an error object to be populated in case of failure.
     * @return The number of bytes sent.
     */
    size_t send_from_isr(const uint8_t* data, size_t size, uint64_t time, error** error = nullptr) OS_NOEXCEPT;

    /**
     * @brief Receives data from the stream buffer.
     *
     * This function receives data from the stream buffer and stores it in the specified buffer.
     * It blocks the caller until the specified amount of data is received or until the specified time has elapsed.
     *
     * @param data Pointer to the buffer where the received data will be stored.
     * @param size The size (in bytes) of the buffer.
     * @param time The maximum time to wait for receiving the data (in milliseconds).
     * @param error Optional pointer to an error object to be populated in case of failure.
     * @return The number of bytes received.
     */
    size_t receive(uint8_t* data, size_t size, uint64_t time, error** error = nullptr) OS_NOEXCEPT;

    /**
     * @brief Receives data from the stream buffer from an ISR.
     *
     * This function is an ISR (Interrupt Service Routine) version of the receive() function.
     * It has the same behavior as receive(), but it is meant to be called from an ISR context.
     *
     * @param data Pointer to the buffer where the received data will be stored.
     * @param size The size (in bytes) of the buffer.
     * @param time The maximum time to wait for receiving the data (in milliseconds).
     * @param error Optional pointer to an error object to be populated in case of failure.
     * @return The number of bytes received.
     */
    size_t receive_from_isr(uint8_t* data, size_t size, uint64_t time, error** error = nullptr) OS_NOEXCEPT;

    /**
     * @brief Resets the stream buffer.
     *
     * This function resets the stream buffer, discarding all the data it contains.
     */
    void reset() OS_NOEXCEPT;

    /**
     * @brief Checks if the stream buffer is empty.
     *
     * @return `true` if the stream buffer is empty, `false` otherwise.
     */
    bool is_empty() const OS_NOEXCEPT;

    /**
     * @brief Checks if the stream buffer is full.
     *
     * @return `true` if the stream buffer is full, `false` otherwise.
     */
    bool is_full() const OS_NOEXCEPT;

    /**
     * @brief Returns the size of the stream buffer.
     *
     * @return The size of the stream buffer in bytes.
     */
    size_t size() const OS_NOEXCEPT;

    /**
     * @brief Returns the number of bytes available in the stream buffer.
     *
     * @return The number of bytes available in the stream buffer.
     */
    size_t bytes_free() const;

private:
    stream_buffer_data sb{}; ///< Internal data for the stream buffer.
};

}
}


