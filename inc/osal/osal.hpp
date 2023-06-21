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

#include <stdint.h>

#include "sys/osal_sys.hpp"

#include "array.hpp"
#include "error.hpp"
#include "event.hpp"
#include "log.hpp"
#include "mutex.hpp"
#include "queue.hpp"
#include "semaphore.hpp"
#include "string.hpp"
#include "thread.hpp"
#include "timer.hpp"

#define OS_NO_COPY_NO_MOVE(cls) \
Singleton<T>(const cls&) = delete; \
Singleton<T>& operator = (const cls&) = delete; \
Singleton<T>(cls&&) = delete; \
Singleton<T>& operator = (cls&&) = delete;


namespace osal
{

inline namespace v1
{

    typedef decltype(nullptr) nullptr_t;

    /**
     * @brief Suspends the execution of the current thread for a specified number of microseconds.
     * @param us The number of microseconds to sleep.
     **/
    void us_sleep(uint64_t us) OS_NOEXCEPT;

    /**
     * @brief Retrieves the current time in microseconds.
     * @return The current time in microseconds.
     **/
    uint64_t get_current_time_us(void) OS_NOEXCEPT;

    /**
     * @brief Retrieves the current tick value.
     * @return The current tick value.
     **/
    tick tick_current(void) OS_NOEXCEPT;

    /**
     * @brief Converts a time value in microseconds to a tick value.
     * @param us The time value in microseconds.
     * @return The corresponding tick value.
     **/
    tick tick_from_us(uint64_t us) OS_NOEXCEPT;

    /**
     * @brief Suspends the execution of the current thread for the specified tick duration.
     * @param tick The tick duration to sleep.
     **/
    void tick_sleep(tick tick) OS_NOEXCEPT;

}
}

namespace os = osal;
