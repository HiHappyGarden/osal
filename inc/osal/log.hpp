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
#include <stdio.h>
#include <stdarg.h>

namespace osal
{
inline namespace v1
{


    constexpr inline const uint8_t LEVEL_DEBUG   = 0x00;
    constexpr inline const uint8_t LEVEL_INFO    = 0x01;
    constexpr inline const uint8_t LEVEL_WARNING = 0x02;
    constexpr inline const uint8_t LEVEL_ERROR   = 0x03;
    constexpr inline const uint8_t LEVEL_FATAL   = 0x04;
    constexpr inline const uint8_t LEVEL_MASK    = 0x0F;

    constexpr inline const uint8_t STATE_ON  = (1 << 7);
    constexpr inline const uint8_t STATE_OFF = (0 << 7);

    constexpr inline uint8_t get_level_log(uint8_t t) { return t & LEVEL_MASK; }
    void set_level_log(uint8_t t) OS_NOEXCEPT;
    void set_enable_log(bool t) OS_NOEXCEPT;
    bool get_enable_log() OS_NOEXCEPT;

    bool is_enabled_log(uint8_t type);

    void log(const char* tag, uint8_t type, const char* fmt, ...);

    template<typename... Args>
    constexpr inline void log_debug(const char* tag, uint8_t type, const char* fmt,  Args... args)
    {
        log(tag, LEVEL_DEBUG, fmt, args...);
    }

    template<typename... Args>
    constexpr inline void log_info(const char* tag, uint8_t type, const char* fmt,  Args... args)
    {
        log(tag, LEVEL_INFO, fmt, args...);
    }

    template<typename... Args>
    constexpr inline void log_warning(const char* tag, uint8_t type, const char* fmt,  Args... args)
    {
        log(tag, LEVEL_WARNING, fmt, args...);
    }

    template<typename... Args>
    constexpr inline void log_error(const char* tag, uint8_t type, const char* fmt,  Args... args)
    {
        log(tag, LEVEL_ERROR, fmt, args...);
    }

    template<typename... Args>
    constexpr inline void log_fatal(const char* tag, uint8_t type, const char* fmt,  Args... args)
    {
        log(tag, LEVEL_FATAL, fmt, args...);
    }

}
}


