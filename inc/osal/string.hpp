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

template <size_t S = 33>
class string
{
    char data[S] = { '\0' };
    size_t data_count = 0;
public:
    string() = default;
    constexpr string(const char(&str)[S]) OS_NOEXCEPT
        : string(str, S - 1)
    {}

    string(const char* str, size_t size) OS_NOEXCEPT
        : data_count(size)
    {
        if(str == nullptr)
        {
            return;
        }
        strncpy(data, str, sizeof(data));
    }

    inline size_t size() const OS_NOEXCEPT
    {
        return sizeof(data) - 1;
    }

    inline size_t count() const OS_NOEXCEPT
    {
        return data_count;
    }

    inline const char* c_str() const OS_NOEXCEPT
    {
        return data;
    }

    template <size_t Sb>
    string<S> operator+(const char(&b)[Sb]) OS_NOEXCEPT
    {
        size_t b_size = strnlen(b, Sb);
        if(data_count + b_size <= size())
        {
            strncpy(data + data_count, b, S - 1);
            data_count += b_size;
        }
        else
        {
            strncpy(data + data_count, b, size() - count());
            data_count = size();
        }
        return *this;
    }


    template <size_t Sb>
    inline string<S> operator+=(const char(&b)[Sb]) OS_NOEXCEPT
    {
        return (*this) + b;
    }

    template <size_t Sb>
    inline bool operator==(const char(&b)[Sb]) OS_NOEXCEPT
    {
        return strncmp(data, b, size()) == 0;
    }

    template <size_t Sb>
    inline bool operator!=(const char(&b)[Sb]) OS_NOEXCEPT
    {
        return strncmp(data, b, size()) != 0;
    }

    string<S> operator+(const char* b) OS_NOEXCEPT
    {
        size_t b_size = strnlen(b, S);
        if(data_count + b_size <= size())
        {
            strncpy(data + data_count, b, S - 1);
            data_count += b_size;
        }
        else
        {
            strncpy(data + data_count, b, size() - count());
            data_count = size();
        }
        return *this;
    }

    inline string<S> operator+=(const char* b) OS_NOEXCEPT
    {
        return (*this) + b;
    }

    inline bool operator==(const char* b) OS_NOEXCEPT
    {
        return strncmp(data, b, size()) == 0;
    }

    inline bool operator!=(const char* b) OS_NOEXCEPT
    {
        return strncmp(data, b, size()) != 0;
    }


    char operator[](int idx)
    {
//        if(idx >= data_count)
//        {
//            return '\0';
//        }
        return data[idx];
    }

    char operator[](int idx) const OS_NOEXCEPT
    {
        if(idx >= data_count)
        {
            return '\0';
        }
        return data[idx];
    }
};


}
}


