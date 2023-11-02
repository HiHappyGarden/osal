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

#include "osal/error.hpp"
#include "osal_sys/osal_sys.hpp"

namespace osal
{
inline namespace v1
{

class event final
{
public:
    event() OS_NOEXCEPT;

    /**
     * @brief Deleted copy constructor.
     */
    event(const event&) = delete;

    /**
     * @brief Deleted copy assignment operator.
     */
    event& operator=(const event&) = delete;

    /**
     * @brief Deleted move constructor.
     */
    event(event&&) = delete;

    /**
     * @brief Deleted move assignment operator.
     */
    event& operator=(event&&) = delete;

    /**
     * @brief Virtual destructor.
     */
    ~event() OS_NOEXCEPT;

    /**
     * @brief Waits for an event to be set.
     *
     * This function blocks the caller until the specified event bits are set or until the specified time has elapsed.
     *
     * @param mask The event mask specifying which bits to wait for.
     * @param value Pointer to a variable where the value of the event will be stored.
     * @param time The maximum time to wait for the event to be set (in milliseconds).
     * @param error Optional pointer to an error object to be populated in case of failure.
     * @return `OK` if the event was set, `KO` if a timeout occurred.
     */
    exit wait(uint32_t mask, uint32_t* value, uint64_t time, error** error = nullptr) OS_NOEXCEPT;

    /**
     * @brief Waits for an event to be set from an ISR.
     *
     * This function is an ISR (Interrupt Service Routine) version of the wait() function.
     * It has the same behavior as wait(), but it is meant to be called from an ISR context.
     *
     * @param mask The event mask specifying which bits to wait for.
     * @param value Pointer to a variable where the value of the event will be stored.
     * @param time The maximum time to wait for the event to be set (in milliseconds).
     * @param error Optional pointer to an error object to be populated in case of failure.
     * @return `true` if the event was set, `false` if a timeout occurred.
     */
    exit wait_from_isr(uint32_t mask, uint32_t* value, uint64_t time, error** error = nullptr) OS_NOEXCEPT;

    /**
     * @brief Sets event bits.
     *
     * This function sets the specified event bits.
     *
     * @param value The event bits to set.
     */
    void set(uint32_t value) OS_NOEXCEPT;

    /**
     * @brief Sets event bits from an ISR.
     *
     * This function is an ISR (Interrupt Service Routine) version of the set() function.
     * It has the same behavior as set(), but it is meant to be called from an ISR context.
     *
     * @param value The event bits to set.
     */
    void set_from_isr(uint32_t value) OS_NOEXCEPT;

    /**
     * @brief Gets the current event value.
     *
     * This function retrieves the current value of the event.
     *
     * @return The current event value.
     */
    uint32_t get() OS_NOEXCEPT;

    /**
     * @brief Gets the current event value from an ISR.
     *
     * This function is an ISR (Interrupt Service Routine) version of the get() function.
     * It has the same behavior as get(), but it is meant to be called from an ISR context.
     *
     * @return The current event value.
     */
    uint32_t get_from_isr() OS_NOEXCEPT;

    /**
     * @brief Clears specific event bits.
     *
     * This function clears the specified event bits.
     *
     * @param value The event bits to clear.
     */
    void clear(uint32_t value) OS_NOEXCEPT;

    /**
     * @brief Clears specific event bits from an ISR.
     *
     * This function is an ISR (Interrupt Service Routine) version of the clear() function.
     * It has the same behavior as clear(), but it is meant to be called from an ISR context.
     *
     * @param value The event bits to clear.
     */
    void clear_from_isr(uint32_t value) OS_NOEXCEPT;

private:
    event_data e;  ///< Internal data for the event.
};


}
}


