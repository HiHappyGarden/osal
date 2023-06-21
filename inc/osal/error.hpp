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

#include <stdlib.h>
#include <stdint.h>

namespace osal
{
inline namespace v1
{
    typedef decltype(nullptr) nullptr_t;

    template <size_t Size>
    class string;

    class error final
    {
        const char* msg = nullptr;
        uint8_t code = 0;
        const char* file = nullptr;
        const char* funct = nullptr;
        uint32_t line = 0;

        osal::error* new_error;
    public:
        error() = default;
        explicit error(nullptr_t) {}
        explicit error(const char* msg, uint8_t code, const char* file, const char* funct, uint32_t line) OS_NOEXCEPT;
        ~error() OS_NOEXCEPT;
        inline void add_error(const error& new_error) OS_NOEXCEPT
        {
            this->new_error = new error(new_error);
        }

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

        inline const char* get_funct() const OS_NOEXCEPT
        {
            return funct;
        }

        inline uint32_t get_line() const OS_NOEXCEPT
        {
            return line;
        }
    };
}
}


