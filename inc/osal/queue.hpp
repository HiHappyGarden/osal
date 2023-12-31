/***************************************************************************
 * 
 * OSAL
 * Copyright (C) 2023/2024 Antonio Salsi <passy.linux@zresa.it>
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

#include "osal/error.hpp"
#include "osal_sys/osal_sys.hpp"

#include <stdlib.h>


namespace osal
{
inline namespace v1
{

/**
 * @brief Final class for queues.
 *
 * This class provides a queue implementation.
 * It is a final class, meaning it cannot be derived from.
 */
class queue final
{
public:
    /**
     * @brief Constructor.
     *
     * @param size The maximum number of messages that can be stored in the queue.
     * @param message_size The size (in bytes) of each message in the queue.
     * @param error Optional pointer to an error object to be populated in case of failure.
     */
    queue(size_t size, size_t message_size, error** error = nullptr) OS_NOEXCEPT;

    /**
     * @brief Deleted copy constructor.
     */
    queue(const queue&) = delete;

    /**
     * @brief Deleted copy assignment operator.
     */
    queue& operator=(const queue&) = delete;

    /**
     * @brief Deleted move constructor.
     */
    queue(queue&&) = delete;

    /**
     * @brief Deleted move assignment operator.
     */
    queue& operator=(queue&&) = delete;

    /**
     * @brief Virtual destructor.
     */
    ~queue() OS_NOEXCEPT;

    /**
     * @brief Fetches a message from the queue.
     *
     * This function retrieves a message from the queue and stores it in the specified `msg` buffer.
     * It blocks the caller until a message is available or until the specified time has elapsed.
     *
     * @param msg Pointer to the buffer where the fetched message will be stored.
     * @param time The maximum time to wait for a message (in milliseconds).
     * @param error Optional pointer to an error object to be populated in case of failure.
     * @return OK if a message was fetched successfully, KO if the fetch timed out or encountered an error.
     */
    osal::exit fetch (void* msg, uint64_t time, error** error = nullptr) OS_NOEXCEPT;

    /**
     * @brief Fetches a message from the queue from an ISR.
     *
     * This function is an ISR (Interrupt Service Routine) version of the fetch() function.
     * It has the same behavior as fetch(), but it is meant to be called from an ISR context.
     *
     * @param msg Pointer to the buffer where the fetched message will be stored.
     * @param time The maximum time to wait for a message (in milliseconds).
     * @param error Optional pointer to an error object to be populated in case of failure.
     * @return OK if a message was fetched successfully, KO if the fetch timed out or encountered an error.
     */
    osal::exit fetch_from_isr (void* msg, uint64_t time, error** error = nullptr) OS_NOEXCEPT;

    /**
     * @brief Posts a message to the queue.
     *
     * This function posts a message to the queue.
     * It blocks the caller until there is space available in the queue or until the specified time has elapsed.
     *
     * @param msg Pointer to the message to be posted.
     * @param time The maximum time to wait for space in the queue (in milliseconds).
     * @param error Optional pointer to an error object to be populated in case of failure.
     * @return OK if the message was posted successfully, KO if the post timed out or encountered an error.
     */
    osal::exit post (const uint8_t* msg, uint64_t time, error** error = nullptr) OS_NOEXCEPT;

    /**
     * @brief Posts a message to the queue from an ISR.
     *
     * This function is an ISR (Interrupt Service Routine) version of the post() function.
     * It has the same behavior as post(), but it is meant to be called from an ISR context.
     *
     * @param msg Pointer to the message to be posted.
     * @param time The maximum time to wait for space in the queue (in milliseconds).
     * @param error Optional pointer to an error object to be populated in case of failure.
     * @return OK if the message was posted successfully, KO if the post timed out or encountered an error.
     */
    osal::exit post_from_isr (const uint8_t* msg, uint64_t time, error** error = nullptr) OS_NOEXCEPT;

    /**
     * @brief Return size of element insert
     * @return number of element
     */
    size_t size () const OS_NOEXCEPT;

private:
    queue_data q{}; ///< Internal data for the queue.
};

}
}


