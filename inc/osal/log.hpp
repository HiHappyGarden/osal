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
#include "osal/types.hpp"
#include "osal-config.h"

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>


#define OS_ANSI_COLOR_RED     "\x1b[31m"
#define OS_ANSI_COLOR_GREEN   "\x1b[32m"
#define OS_ANSI_COLOR_YELLOW  "\x1b[33m"
#define OS_ANSI_COLOR_BLUE    "\x1b[34m"
#define OS_ANSI_COLOR_MAGENTA "\x1b[35m"
#define OS_ANSI_COLOR_CYAN    "\x1b[36m"
#define OS_ANSI_COLOR_RESET   "\x1b[0m"


#ifndef OS_LOG_DEBUG
#define OS_LOG_DEBUG(tag, ...) osal::log_debug (tag, __VA_ARGS__)
#endif

#ifndef OS_LOG_INFO
#define OS_LOG_INFO(tag, ...) osal::log_info (tag, __VA_ARGS__)
#endif

#ifndef OS_LOG_WARNING
#define OS_LOG_WARNING(tag, ...) osal::log_warning (tag, __VA_ARGS__)
#endif

#ifndef OS_LOG_ERROR
#define OS_LOG_ERROR(tag, ...) osal::log_error (tag, __VA_ARGS__)
#endif

#ifndef OS_LOG_FATAL
#define OS_LOG_FATAL(tag, ...) osal::log_fatal (tag, __VA_ARGS__)
#endif

#ifndef OS_LOG_PRINTF
#define OS_LOG_PRINTF(...) printf (__VA_ARGS__)
#endif

#ifndef OS_LOG_NEW_LINE
#define OS_LOG_NEW_LINE "\r\n"
#endif

namespace osal
{
inline namespace v1
{

/**
 * @brief Debug log level.
 */
constexpr inline const uint8_t LEVEL_DEBUG   = 0x01;

/**
 * @brief Info log level.
 */
constexpr inline const uint8_t LEVEL_INFO    = 0x02;

/**
 * @brief Warning log level.
 */
constexpr inline const uint8_t LEVEL_WARNING = 0x04;

/**
 * @brief Error log level.
 */
constexpr inline const uint8_t LEVEL_ERROR   = 0x08;

/**
 * @brief Fatal log level.
 */
constexpr inline const uint8_t LEVEL_FATAL   = 0x10;

/**
 * @brief Log level mask.
 */
constexpr inline const uint8_t LEVEL_MASK    = 0x1F;

/**
 * @brief Log state when enabled.
 */
constexpr inline const uint8_t LOG_STATE_ON  = (1 << 7);

/**
 * @brief Log state when disabled.
 */
constexpr inline const uint8_t LOG_STATE_OFF = (0 << 7);

/**
 * @brief Gets the log level from a given value.
 *
 * @param t The value containing the log level.
 * @return The log level extracted from the value.
 */
constexpr inline uint8_t get_level_log(uint8_t t) OS_NOEXCEPT { return t & LEVEL_MASK; }

/**
 * @brief Sets the log level.
 *
 * @param t The log level to set.
 */
void set_level_log(uint8_t t) OS_NOEXCEPT;

/**
 * @brief Sets the log enable state.
 *
 * @param t The enable state of the log.
 */
void set_enable_log(bool t) OS_NOEXCEPT;

/**
 * @brief Gets the log enable state.
 *
 * @return The enable state of the log.
 */
bool get_enable_log() OS_NOEXCEPT;

/**
 * @brief Checks if a specific log level is enabled.
 *
 * @param type The log level to check.
 * @return True if the log level is enabled, false otherwise.
 */
bool is_enabled_log(uint8_t type) OS_NOEXCEPT;

/**
 * @brief Writes a log message with the given tag, log level, and formatted message.
 *
 * @param tag The tag of the log message.
 * @param type The log level.
 * @param fmt The format string for the log message.
 * @param ... The variadic arguments for formatting the log message.
 */
void sys_log(const char* tag, uint8_t type, const char* fmt, ...) OS_NOEXCEPT;

/**
 * @brief Writes a debug log message with the given tag and formatted message.
 *
 * @tparam Args The variadic argument types.
 * @param tag The tag of the log message.
 * @param fmt The format string for the log message.
 * @param args The additional arguments for formatting the log message.
 */
template<typename... Args>
constexpr inline void log_debug(const char* tag, const char* fmt, Args... args) OS_NOEXCEPT
{
    sys_log(tag, LEVEL_DEBUG, fmt, args...);
}

/**
 * @brief Writes an info log message with the given tag and formatted message.
 *
 * @tparam Args The variadic argument types.
 * @param tag The tag of the log message.
 * @param fmt The format string for the log message.
 * @param args The additional arguments for formatting the log message.
 */
template<typename... Args>
constexpr inline void log_info(const char* tag, const char* fmt, Args... args) OS_NOEXCEPT
{
    sys_log(tag, LEVEL_INFO, fmt, args...);
}

/**
 * @brief Writes a warning log message with the given tag and formatted message.
 *
 * @tparam Args The variadic argument types.
 * @param tag The tag of the log message.
 * @param fmt The format string for the log message.
 * @param args The additional arguments for formatting the log message.
 */
template<typename... Args>
constexpr inline void log_warning(const char* tag, const char* fmt, Args... args) OS_NOEXCEPT
{
    sys_log(tag, LEVEL_WARNING, fmt, args...);
}

/**
 * @brief Writes an error log message with the given tag and formatted message.
 *
 * @tparam Args The variadic argument types.
 * @param tag The tag of the log message.
 * @param fmt The format string for the log message.
 * @param args The additional arguments for formatting the log message.
 */
template<typename... Args>
constexpr inline void log_error(const char* tag, const char* fmt, Args... args) OS_NOEXCEPT
{
    sys_log(tag, LEVEL_ERROR, fmt, args...);
}

/**
 * @brief Writes a fatal log message with the given tag and formatted message.
 *
 * @tparam Args The variadic argument types.
 * @param tag The tag of the log message.
 * @param fmt The format string for the log message.
 * @param args The additional arguments for formatting the log message.
 */
template<typename... Args>
constexpr inline void log_fatal(const char* tag, const char* fmt, Args... args) OS_NOEXCEPT
{
    sys_log(tag, LEVEL_FATAL, fmt, args...);
}

/**
 * @brief Resets the log color settings.
 */
inline void reset_color_log() OS_NOEXCEPT
{
    printf(OS_ANSI_COLOR_RESET "");
    fflush(stdout);
}


}
}

extern "C" uint64_t osal_system_current_time_millis() __attribute__((weak));
