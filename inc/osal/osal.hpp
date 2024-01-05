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

#include <stdint.h>

#include "osal_sys/osal_sys.hpp"

#include "osal/array.hpp"
#include "osal/config.h"
#include "osal/error.hpp"
#include "osal/event.hpp"
#include "osal/generics.hpp"
#include "osal/iterator.hpp"
#include "osal/log.hpp"
#include "osal/memory.hpp"
#include "osal/mutex.hpp"
#include "osal/queue.hpp"
#include "osal/semaphore.hpp"
#include "osal/streambuffer.hpp"
#include "osal/string.hpp"
#include "osal/thread.hpp"
#include "osal/timer.hpp"
#include "osal/traits.hpp"
#include "osal/types.hpp"

#define OS_NO_COPY_NO_MOVE(clazz) \
clazz(const clazz&) = delete; \
clazz& operator=(const clazz&) = delete; \
clazz(clazz&&) = delete; \
clazz& operator=(clazz&&) = delete;

#define OS_UNUSED(expr) do { (void)(expr); } while (0)


namespace osal
{

inline namespace v1
{

constexpr inline uint64_t ms_to_us(uint32_t millis) { return millis * 1'000ul; }
constexpr inline uint64_t sec_to_us(uint32_t sec) { return sec * 1'000'000ul; }

constexpr inline uint64_t operator""_ms( unsigned long long ms ) {  return ms_to_us(ms); }
constexpr inline uint64_t operator""_s( unsigned long long secs ) {  return sec_to_us(secs); }



/**
  * @brief Suspends the execution of the current thread for a specified number of microseconds.
  * @param us The number of microseconds to sleep.
  **/
[[maybe_unused]] void us_sleep(uint64_t us) OS_NOEXCEPT;

/**
  * @brief Retrieves the current time in microseconds.
  * @return The current time in microseconds.
  **/
[[maybe_unused]] uint64_t get_current_time_us() OS_NOEXCEPT;

/**
  * @brief Retrieves the current tick value.
  * @return The current tick value.
  **/
[[maybe_unused]] tick tick_current() OS_NOEXCEPT;

/**
  * @brief Converts a time value in microseconds to a tick value.
  * @param us The time value in microseconds.
  * @return The corresponding tick value.
  **/
[[maybe_unused]] tick tick_from_us(uint64_t us) OS_NOEXCEPT;

/**
  * @brief Suspends the execution of the current thread for the specified tick duration.
  * @param tick The tick duration to sleep.
  **/
[[maybe_unused]] void tick_sleep(tick tick) OS_NOEXCEPT;

/**
 * @brief Sets the main loop to sleep mode.
 *
 * This function sets the main loop to sleep mode, where the main application logic will be suspended or put into a low-power state.
 * The main loop will remain in the sleep mode for the specified sleep timing, after which it will resume normal execution.
 *
 * @param sleep_timing The time duration (in milliseconds) for the main loop to remain in sleep mode.
 */
[[maybe_unused]] void set_sleep_main_loop(uint16_t sleep_timing) OS_NOEXCEPT;

/**
 * @brief Gets the sleep mode timing of the main loop.
 *
 * This function retrieves the sleep mode timing value that was previously set for the main loop.
 *
 * @return The time duration (in milliseconds) for the main loop to remain in sleep mode.
 */
[[maybe_unused]] uint16_t get_sleep_main_loop() OS_NOEXCEPT;

/**
 * @brief Set this valie permit to exape onlyone temporization in main loop
 * @param check
 */
[[maybe_unused]] void set_check_main_loop(bool check) OS_NOEXCEPT;

/**
 * @brief Starts the main loop.
 *
 * This function starts the main loop, which is responsible for the continuous execution of the main application logic.
 * The main loop will run until it is explicitly stopped or an exception occurs.
 */
[[maybe_unused]] void start_main_loop() OS_NOEXCEPT;

/**
 * @brief Stops the main loop.
 *
 * This function stops the main loop, which terminates the continuous execution of the main application logic.
 * After calling this function, the main loop will exit and the application will be in a stopped state.
 */
[[maybe_unused]] void stop_main_loop() OS_NOEXCEPT;

}
}

namespace os = osal;

extern "C"
{
[[maybe_unused]] void osal_us_sleep(uint64_t us);
[[maybe_unused]] uint64_t osal_ms_to_us(uint32_t millis);
[[maybe_unused]] uint64_t osal_sec_to_us(uint32_t sec);
}

