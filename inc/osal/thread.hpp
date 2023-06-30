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

    class thread {
    public:
        thread() = default;
        thread(const thread&) = delete;
        thread& operator = (const thread&) = delete;
        thread(thread&&) = delete;
        thread& operator = (thread&&) = delete;

        /**
      * @typedef handler
      * @brief Function pointer type for the thread handler.
      * @param arg The argument passed to the thread handler.
      */
#ifdef __MACH__
        using handler = void* _Nullable (* _Nonnull)(void* _Nullable);
#else
        using handler = void* (*)(void* arg);
#endif

        /**
      * @brief Default destructor for the thread class.
      */
        virtual ~thread() OS_NOEXCEPT = default;

        /**
      * @brief Creates a new thread.
      * @param arg The argument to be passed to the thread handler.
      * @return true if the thread was created successfully, false otherwise.
      */
#ifdef __MACH__
        virtual bool create(void* _Nullable arg = nullptr) OS_NOEXCEPT = 0;
#else
        virtual bool create(void* arg = nullptr, class error** error = nullptr) OS_NOEXCEPT = 0;
#endif
        /**
      * @brief Builds a new thread object.
      * @param name The name of the thread.
      * @param priority The priority of the thread.
      * @param stack_size The stack size of the thread.
      * @param handler The thread handler function.
      * @return A pointer to the newly created thread object.
      */

#ifdef __MACH__
        static thread* _Nullable build(const char* _Nullable name, uint32_t priority, size_t stack_size, handler handler, class error** error = nullptr) OS_NOEXCEPT;
#else
        static thread* build(const char* name, uint32_t priority, size_t stack_size, handler handler) OS_NOEXCEPT;
#endif

        virtual bool exit() OS_NOEXCEPT = 0;
    };


}
}


