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

#include <stdlib.h>


namespace osal
{
inline namespace v1
{

class queue final
{
public:
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

    bool fetch (void * msg, uint64_t time, error** error) OS_NOEXCEPT;

    bool fetch_from_isr (void * msg, uint64_t time, error** error) OS_NOEXCEPT;

    bool post (const uint8_t* msg, uint64_t time, error** error) OS_NOEXCEPT;

    bool post_from_isr (const uint8_t* msg, uint64_t time, error** error) OS_NOEXCEPT;

private:
    queue_data q{0};
};

}
}


