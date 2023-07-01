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

#include <stdint.h>
#include <stdlib.h>

/**
 *  @namespace osal
 *  @brief The namespace for the Operating System Abstraction Layer (OSAL).
*/
namespace osal
{
inline namespace v1
{


    /**
    * @class thread
    * @brief Represents a thread abstraction.
    */

    class thread final
    {
    public:

        /**
        * @typedef handler
        * @brief Function pointer type for the thread handler.
        * @param arg The argument passed to the thread handler.
        */
        using handler = void* (*)(void* arg);

        thread(const char *name, uint32_t priority, size_t stack_size, handler handler);
        thread(const thread&) = delete;
        thread& operator = (const thread&) = delete;
        thread(thread&&) = delete;
        thread& operator = (thread&&) = delete;


        /**
        * @brief Default destructor for the thread class.
        */
        ~thread() OS_NOEXCEPT = default;

        /**
        * @brief Creates a new thread.
        * @param arg The argument to be passed to the thread handler.
        * @return true if the thread was created successfully, false otherwise.
        */
        bool create(void* arg = nullptr, class error** error = nullptr) OS_NOEXCEPT;

        /**
         * @brief Exit from thread.
         * @return true OK
         * @return false KO
         */
        bool exit() OS_NOEXCEPT;

        bool join(error** error);



    private:
        char name[33];
        uint32_t priority;
        size_t stack_size;
        thread::handler h;

        thread_data t {0};
    };


}
}


