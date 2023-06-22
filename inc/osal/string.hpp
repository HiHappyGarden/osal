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
#include <string.h>
namespace osal
{
inline namespace v1
{

    template <size_t Size = 33>
    class string
    {
        char data[Size]{};
        size_t data_length{0};
    public:
        string() = default;
        constexpr string(const char(&str)[Size]) OS_NOEXCEPT
            : string(str, Size - 1)
        {}

        constexpr string(const char* str, size_t size) OS_NOEXCEPT
            : data_length(size)
        {
            if(str == nullptr)
            {
                return;
            }
            strncpy(data, str, sizeof(data));
        }

        inline ~string() OS_NOEXCEPT { memset(&data, '\0', sizeof(data)); }

        constexpr inline size_t size() const OS_NOEXCEPT
        {
            return sizeof(data) - 1;
        }

        constexpr inline size_t length() const OS_NOEXCEPT
        {
            return data_length;
        }

        constexpr inline const char* c_str() const OS_NOEXCEPT
        {
            return data;
        }

        template <size_t SizeB>
        constexpr string<Size>&operator+(const char(&b)[SizeB]) OS_NOEXCEPT
        {
            size_t size_b = strnlen(b, SizeB);
            if(data_length + size_b <= size())
            {
                strncpy(data + data_length, b, Size - 1);
                data_length += size_b;
            }
            else
            {
                strncpy(data + data_length, b, size() - length());
                data_length = size();
            }
            return *this;
        }

        template <size_t SizeB>
        constexpr inline string<Size> operator+(char(&&b)[SizeB]) OS_NOEXCEPT
        {
            return (*this) + b;
        }

        template <size_t SizeB>
        constexpr inline string<Size>& operator+=(char(&&b)[SizeB]) OS_NOEXCEPT
        {
            return (*this) + b;
        }

        template <size_t SizeB>
        constexpr inline string<Size>& operator+=(const char(&b)[SizeB]) OS_NOEXCEPT
        {
            return (*this) + b;
        }

        template <size_t SizeB>
        constexpr inline bool operator==(char(&&b)[SizeB]) OS_NOEXCEPT
        {
            return strncmp(data, b, size()) == 0;
        }

        template <size_t SizeB>
        constexpr inline bool operator==(const char(&b)[SizeB]) OS_NOEXCEPT
        {
            return strncmp(data, b, size()) == 0;
        }

        template <size_t SizeB>
        constexpr inline bool operator!=(const char(&b)[SizeB]) OS_NOEXCEPT
        {
            return strncmp(data, b, size()) != 0;
        }

        template <size_t SizeB>
        constexpr inline bool operator!=(char(&&b)[SizeB]) OS_NOEXCEPT
        {
            return strncmp(data, b, size()) != 0;
        }


        constexpr string<Size>& operator+(const char* b) OS_NOEXCEPT
        {
            size_t size_b = strnlen(b, Size);
            if(data_length + size_b <= size())
            {
                strncpy(data + data_length, b, Size - 1);
                data_length += size_b;
            }
            else
            {
                strncpy(data + data_length, b, size() - length());
                data_length = size();
            }
            return *this;
        }

        constexpr inline string<Size>& operator+=(const char* b) OS_NOEXCEPT
        {
            return (*this) + b;
        }

        constexpr inline bool operator==(const char* b) OS_NOEXCEPT
        {
            return strncmp(data, b, size()) == 0;
        }

        constexpr inline bool operator!=(const char* b) OS_NOEXCEPT
        {
            return strncmp(data, b, size()) != 0;
        }

        constexpr char* operator[](size_t idx) OS_NOEXCEPT
        {
            if(idx >= data_length)
            {
                return nullptr;
            }
            return &data[idx];
        }

        constexpr const char* operator[](size_t idx) const OS_NOEXCEPT
        {
            if(idx >= data_length)
            {
                return nullptr;
            }
            return &data[idx];
        }

        constexpr string<Size>& operator=(const char* other) OS_NOEXCEPT
        {
            if(other == nullptr)
            {
                return *this;
            }
            memset(data, '\0', sizeof(data));
            data_length = 0;
            (*this) += other;
            return *this;
        }
    };


}
}


