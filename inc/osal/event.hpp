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

        bool wait (uint32_t mask, uint32_t * value, uint64_t time, error** error = nullptr) OS_NOEXCEPT;

        bool wait_from_isr (uint32_t mask, uint32_t * value, uint64_t time, error** error = nullptr) OS_NOEXCEPT;

        void set (uint32_t value) OS_NOEXCEPT;

        void set_from_isr (uint32_t value) OS_NOEXCEPT;

        uint32_t get() OS_NOEXCEPT;

        uint32_t get_from_isr() OS_NOEXCEPT;

        void clear (uint32_t value) OS_NOEXCEPT;

        void clear_from_isr (uint32_t value) OS_NOEXCEPT;
    private:
        event_data e;
    };


}
}


