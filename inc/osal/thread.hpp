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
 *
 * This class provides an abstraction for creating and managing threads.
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

    /**
     * @brief Constructor for the thread class.
     *
     * @param name The name of the thread.
     * @param priority The priority of the thread.
     * @param stack_size The stack size of the thread.
     * @param handler The function pointer to the thread handler.
     */
    thread(const char* name, uint32_t priority, size_t stack_size, handler handler) OSAL_NOEXCEPT;

    inline thread(const char* name, uint32_t priority, size_t stack_size, handler handler, void* arg, class error** error = nullptr) OSAL_NOEXCEPT
    : thread(name, priority, stack_size, handler)
    {
        create(arg, error);
    }

    /**
     * @brief Deleted copy constructor.
     */
    thread(const thread&) = delete;

    /**
     * @brief Deleted copy assignment operator.
     */
    thread& operator=(const thread&) = delete;

    /**
     * @brief Deleted move constructor.
     */
    thread(thread&&) = delete;

    /**
     * @brief Deleted move assignment operator.
     */
    thread& operator=(thread&&) = delete;

    /**
     * @brief Default destructor for the thread class.
     */
    ~thread() OSAL_NOEXCEPT = default;

    /**
     * @brief Creates a new thread.
     *
     * @param arg The argument to be passed to the thread handler.
     * @param error Optional pointer to an error object to be populated in case of failure.
     * @return `true` if the thread was created successfully, `false` otherwise.
     */
    osal::exit create(void* arg = nullptr, class error** error = nullptr) OSAL_NOEXCEPT;

    /**
     * @brief Exits from the thread.
     *
     * @return `true` if the thread was successfully exited, `false` otherwise.
     */
    osal::exit exit() OSAL_NOEXCEPT;

    /**
     * @brief Joins the thread.
     *
     * This function waits for the thread to complete its execution.
     *
     * @param error Optional pointer to an error object to be populated in case of failure.
     * @return `true` if the thread was successfully joined, `false` otherwise.
     */
    osal::exit join(error** error = nullptr) const OSAL_NOEXCEPT;

private:
    char name[33]{};                  ///< The name of the thread.
    uint32_t priority;              ///< The priority of the thread.
    size_t stack_size;              ///< The stack size of the thread.
    thread::handler h;              ///< The thread handler function.

    thread_data t {};              ///< Internal data for the thread.
};


}
}


