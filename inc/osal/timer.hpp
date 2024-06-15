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

#include <stdint.h>

namespace osal
{
inline namespace v1
{

/**
 * @brief Final class for timers.
 *
 * This class provides a timer implementation.
 * It is a final class, meaning it cannot be derived from.
 */
class timer final
{
public:
    /**
     * @brief Function pointer type for the timer handler.
     *
     * @param timer Pointer to the timer object.
     * @param arg The argument passed to the timer handler.
     */
    using handler = void* (*)(timer*, void*);

    /**
     * @brief Constructor.
     *
     * @param us The time interval for the timer (in microseconds).
     * @param fn The handler function to be called when the timer expires.
     * @param one_shot Flag indicating whether the timer is a one-shot timer (default: false).
     */
    timer(uint64_t us, handler fn, bool one_shot = false) OSAL_NOEXCEPT;

    /**
     * @brief Deleted copy constructor.
     */
    timer(const timer&) = delete;

    /**
     * @brief Deleted copy assignment operator.
     */
    timer& operator=(const timer&) = delete;

    /**
     * @brief Deleted move constructor.
     */
    timer(timer&&) = delete;

    /**
     * @brief Deleted move assignment operator.
     */
    timer& operator=(timer&&) = delete;

    /**
     * @brief Destructor.
     */
    ~timer() OSAL_NOEXCEPT;

    /**
     * @brief Creates the timer.
     *
     * @param arg The argument to be passed to the timer handler.
     * @param error Optional pointer to an error object to be populated in case of failure.
     * @return `true` if the timer was created successfully, `false` otherwise.
     */
    osal::exit create(void* arg = nullptr, error** error = nullptr) OSAL_NOEXCEPT;

    /**
     * @brief Sets the time interval for the timer.
     *
     * @param us The time interval for the timer (in microseconds).
     */
    void set(uint64_t us) OSAL_NOEXCEPT;

    /**
     * @brief Sets the time interval for the timer from an ISR.
     *
     * @param us The time interval for the timer (in microseconds).
     */
    void set_from_isr(uint64_t us) OSAL_NOEXCEPT;

    /**
     * @brief Starts the timer.
     */
    void start() const OSAL_NOEXCEPT;

    /**
     * @brief Starts the timer from an ISR.
     *
     * This function is an ISR (Interrupt Service Routine) version of the start() function.
     * It has the same behavior as start(), but it is meant to be called from an ISR context.
     */
    void start_from_isr() const OSAL_NOEXCEPT;

    /**
     * @brief Stops the timer.
     */
    void stop() const OSAL_NOEXCEPT;

    /**
     * @brief Stops the timer from an ISR.
     *
     * This function is an ISR (Interrupt Service Routine) version of the stop() function.
     * It has the same behavior as stop(), but it is meant to be called from an ISR context.
     */
    void stop_from_isr() const OSAL_NOEXCEPT;

    bool is_active() const OSAL_NOEXCEPT;

private:
    uint64_t us;    ///< The time interval for the timer (in microseconds).
    handler fn;     ///< The handler function to be called when the timer expires.
    bool one_shot;   ///< Flag indicating whether the timer is a one-shot timer.
    timer_data t{};   ///< Internal data for the timer.

    friend void* timer_thread(void*);

};
}
}

