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

    template <typename T, typename... Args>
    class array
    {
        size_t data_idx{0};
        T data[sizeof... (Args)]{};

        template <typename TP>
        void fill(const TP& last) OS_NOEXCEPT
        {
            memcpy(&data[data_idx], &last, sizeof (last));
        }

        template <typename TP, typename... ArgsP>
        void fill(const TP& first, ArgsP... args) OS_NOEXCEPT
        {
            memcpy(&data[data_idx], &first, sizeof (first));
            data_idx++;
            fill(args...);
        }

    public:
        explicit array(const T& first, Args... args) OS_NOEXCEPT
        {
            fill(first, args...);
        }


        inline const T* get_data() const OS_NOEXCEPT
        {
            return &data[0];
        }

    };


}
}


