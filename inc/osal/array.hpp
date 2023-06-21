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

    template <typename T, size_t Size>
    class array final
    {
    private:
        T data[Size]{0};
        size_t data_length{0};

    public:
        constexpr array() = default;
        inline ~array() OS_NOEXCEPT { memset(&data, 0, sizeof(data)); }
        constexpr inline const T* get_data() const OS_NOEXCEPT
        {
            return &data[0];
        }

        constexpr inline T* get_data() OS_NOEXCEPT
        {
            return &data[0];
        }

        constexpr inline size_t size() const OS_NOEXCEPT
        {
            return Size;
        }

        constexpr inline size_t length() const OS_NOEXCEPT
        {
            return data_length;
        }

        constexpr array<T, Size> & operator<<(const T& other) OS_NOEXCEPT
        {
            if(data_length + 1 <= size())
            {
                memcpy(data + data_length, &other, sizeof(T));
                data_length += 1;
            }
            return *this;
        }

        constexpr inline array<T, Size>& operator<<(T&& other) OS_NOEXCEPT
        {
            return (*this) << other;
        }

        constexpr T& operator[](size_t idx) OS_NOEXCEPT
        {
            if(idx >= data_length)
            {
                //exit(0);
            }
            return data[idx];
        }

        constexpr const T& operator[](size_t idx) const OS_NOEXCEPT
        {
            if(idx >= data_length)
            {
                //exit(0);
            }
            return data[idx];
        }

    };


    template <typename T, typename... Args>
    class array_init final
    {
        size_t data_length{0};
        T data[sizeof... (Args) + 1]{0};


        template <typename TP>
        constexpr void fill(const TP& last) OS_NOEXCEPT
        {
            memcpy(&data[data_length], &last, sizeof (last));
            data_length++;
        }

        template <typename TP, typename... ArgsP>
        constexpr void fill(const TP& first, ArgsP... args) OS_NOEXCEPT
        {
            memcpy(&data[data_length], &first, sizeof (first));
            data_length++;
            fill(args...);
        }

    public:
        constexpr array_init() = default;
        inline ~array_init() { memset(&data, 0, sizeof(data)); }
        constexpr explicit array_init(const T& first, Args... args) OS_NOEXCEPT
        {
            fill(first, args...);
        }

        constexpr inline const T* get_data() const OS_NOEXCEPT
        {
            return &data[0];
        }

        constexpr inline T* get_data() OS_NOEXCEPT
        {
            return &data[0];
        }

        constexpr inline size_t size() const OS_NOEXCEPT
        {
            return sizeof(data) / sizeof (data[0]);
        }

        constexpr inline size_t length() const OS_NOEXCEPT
        {
            return data_length;
        }

        constexpr array_init<T, Args...>& operator<<(const T& other) OS_NOEXCEPT
        {
            if(data_length + 1 <= size())
            {
                memcpy(data + data_length, &other, sizeof(T));
                data_length += 1;
            }
            return *this;
        }

        constexpr array_init<T, Args...>& operator<<(T&& other) OS_NOEXCEPT
        {
            return (*this) << other;
        }

        constexpr T& operator[](size_t idx) OS_NOEXCEPT
        {
            if(idx >= data_length)
            {
                //exit(0);
            }
            return data[idx];
        }

        constexpr const T& operator[](size_t idx) const OS_NOEXCEPT
        {
            if(idx >= data_length)
            {
                //exit(0);
            }
            return data[idx];
        }
    };


}
}


