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

namespace osal
{
inline namespace v1
{

/**
 * @brief Abstract base class for mutexes.
 *
 * This class provides an abstract interface for mutexes.
 * It cannot be instantiated directly and should be used as a base class for concrete mutex implementations.
 * The class defines pure virtual functions for locking and unlocking the mutex.
 *
 * @note This class is non-copyable and non-movable.
 */
class mutex
{
public:
    /**
     * @brief Default constructor.
     */
    mutex() = default;

    /**
     * @brief Deleted copy constructor.
     */
    mutex(const mutex&) = delete;

    /**
     * @brief Deleted copy assignment operator.
     */
    mutex& operator=(const mutex&) = delete;

    /**
     * @brief Deleted move constructor.
     */
    mutex(mutex&&) = delete;

    /**
     * @brief Deleted move assignment operator.
     */
    mutex& operator=(mutex&&) = delete;

    /**
     * @brief Virtual destructor.
     */
    ~mutex() OS_NOEXCEPT;

    /**
     * @brief Locks the mutex.
     *
     * This function is pure virtual and should be implemented by derived classes to lock the mutex.
     */
    virtual void lock() OS_NOEXCEPT = 0;

    /**
     * @brief Unlocks the mutex.
     *
     * This function is pure virtual and should be implemented by derived classes to unlock the mutex.
     */
    virtual void unlock() OS_NOEXCEPT = 0;

#ifdef __MACH__
    /**
     * @brief Builds a mutex object.
     *
     * This static function creates a mutex object of the appropriate type for the platform.
     *
     * @param error Optional pointer to an error object to be populated in case of failure (macOS only).
     * @return A pointer to the created mutex object, or nullptr on failure.
     */
    static mutex* _Nullable build(class error** error = nullptr) OS_NOEXCEPT;
#else
    /**
     * @brief Builds a mutex object.
     *
     * This static function creates a mutex object of the appropriate type for the platform.
     *
     * @param error Optional pointer to an error object to be populated in case of failure.
     * @return A pointer to the created mutex object, or nullptr on failure.
     */
    static mutex* build(class error** error = nullptr) OS_NOEXCEPT;
#endif
};


}
}


