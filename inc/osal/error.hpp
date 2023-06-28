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

#include "types.hpp"

#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define OSAL_BUILD_ERROR(msg, code) osal::error(msg, code, osal::get_file_name(__FILE__), __FUNCTION__, __LINE__)

namespace osal
{
inline namespace v1
{

    template <typename T, size_t S>
    inline constexpr size_t get_file_name_offset(const T (& str)[S], size_t i = S - 1)
    {
        return (str[i] == '/' || str[i] == '\\') ? i + 1 : (i > 0 ? get_file_name_offset(str, i - 1) : 0);
    }

    template <typename T>
    inline constexpr size_t get_file_name_offset(T (& str)[1])
    {
        return 0;
    }

    template <typename T, size_t S>
    inline constexpr size_t get_file_name_size(const T (& str)[S], size_t i = S - 1)
    {
        return strlen(str) - get_file_name_offset(str);
    }


    template <typename T, size_t S>
    inline constexpr const char* get_file_name(const T (& str)[S])
    {
        return str + get_file_name_offset(str);
    }

    template <size_t Size>
    class string;



    class error final
    {
        const char* msg = nullptr;
        uint8_t code = 0;
        const char* file = nullptr;
        const char* func = nullptr;
        uint32_t line = 0;

        osal::error* old_error = nullptr;

        friend void printf_stack_error(const error &e, const char* fmt, ...);

    public:
        error() = default;
        explicit error(nullptr_t) OS_NOEXCEPT {}
        explicit error(const char* msg, uint8_t code = 0, const char* file = get_file_name(__FILE__), const char* funct = "", uint32_t line = __LINE__) OS_NOEXCEPT;
        error(const error& old_error, const char* msg, uint8_t code = 0, const char* file = get_file_name(__FILE__), const char* funct = "", uint32_t line = __LINE__) OS_NOEXCEPT;
        
        error(const error&) = default;
        error& operator = (const error&) = default;
        error(error&&) = default;
        error& operator = (error&&) = default;

        ~error() OS_NOEXCEPT;

        void add_error(const error& old_error);

        inline const char* get_msg() const OS_NOEXCEPT
        {
            return msg;
        }

        inline uint8_t get_code() const OS_NOEXCEPT
        {
            return code;
        }

        inline const char* get_file() const OS_NOEXCEPT
        {
            return file;
        }

        inline const char* get_func() const OS_NOEXCEPT
        {
            return func;
        }

        inline uint32_t get_line() const OS_NOEXCEPT
        {
            return line;
        }
    };


    template<typename... Args>
    constexpr inline void printf_stack_error(const error &e, Args... args)
    {
        printf_stack_error(e, "",  args...);
    }


    template<typename... Args>
    constexpr inline void printf_stack_error(error &&e, const char* fmt = nullptr,  Args... args)
    {
        printf_stack_error(e, fmt,  args...);
    }
}
}


