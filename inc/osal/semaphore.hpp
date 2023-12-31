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

#include "error.hpp"
#include "osal_sys/osal_sys.hpp"

#include <stdlib.h>

namespace osal
{
inline namespace v1
{

/**
 * @brief Final class for semaphores.
 *
 * This class provides a semaphore implementation.
 * It is a final class, meaning it cannot be derived from.
 *
 * @note This class is non-copyable and non-movable.
 */
class semaphore final
{
public:
    /**
     * @brief Explicit constructor.
     *
     * @param count The initial count for the semaphore.
     */
    explicit semaphore(size_t count, error** error = nullptr) OS_NOEXCEPT;

    /**
     * @brief Deleted copy constructor.
     */
    semaphore(const semaphore&) = delete;

    /**
     * @brief Deleted copy assignment operator.
     */
    semaphore& operator=(const semaphore&) = delete;

    /**
     * @brief Deleted move constructor.
     */
    semaphore(semaphore&&) = delete;

    /**
     * @brief Deleted move assignment operator.
     */
    semaphore& operator=(semaphore&&) = delete;

    /**
     * @brief Virtual destructor.
     */
    ~semaphore() OS_NOEXCEPT;

    /**
     * @brief Waits for the semaphore to become available.
     *
     * This function blocks the caller until the semaphore becomes available or until the specified time has elapsed.
     *
     * @param time The maximum time to wait for the semaphore (in milliseconds).
     * @param error Optional pointer to an error object to be populated in case of failure.
     * @return OK if the semaphore became available, KO if the wait timed out or encountered an error.
     */
    osal::exit wait(uint64_t time, error** error = nullptr) OS_NOEXCEPT;

    /**
     * @brief Waits for the semaphore to become available from an ISR.
     *
     * This function is an ISR (Interrupt Service Routine) version of the wait() function.
     * It has the same behavior as wait(), but it is meant to be called from an ISR context.
     *
     * @param time The maximum time to wait for the semaphore (in milliseconds).
     * @param error Optional pointer to an error object to be populated in case of failure.
     * @return OK if the semaphore became available, KO if the wait timed out or encountered an error.
     */
    osal::exit wait_from_isr(uint64_t time, error** error = nullptr) OS_NOEXCEPT;

    /**
     * @brief Signals the semaphore.
     *
     * This function increments the count of the semaphore, allowing waiting threads to proceed.
     */
    void signal() OS_NOEXCEPT;

    /**
     * @brief Signals the semaphore from an ISR.
     *
     * This function is an ISR (Interrupt Service Routine) version of the signal() function.
     * It has the same behavior as signal(), but it is meant to be called from an ISR context.
     */
    void signal_from_isr() OS_NOEXCEPT;

private:
    semaphore_data sem{}; ///< Internal data for the semaphore.
};

}
}


