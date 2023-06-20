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
    class array
    {
    protected:
        T data[Size]{0};

    public:
        constexpr array() = default;
        inline ~array() { memset(&data, 0, sizeof(data)); }
        constexpr inline const T* get_data() const OS_NOEXCEPT
        {
            return &data[0];
        }

    };


    template <typename T, typename... Args>
    class array_init : public array<T, sizeof... (Args)>
    {
        size_t data_idx{0};
        T data[sizeof... (Args)]{0};

        template <typename TP>
        constexpr void fill(const TP& last) OS_NOEXCEPT
        {
            memcpy(&data[data_idx], &last, sizeof (last));
        }

        template <typename TP, typename... ArgsP>
        constexpr void fill(const TP& first, ArgsP... args) OS_NOEXCEPT
        {
            memcpy(&data[data_idx], &first, sizeof (first));
            data_idx++;
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

    };


}
}


