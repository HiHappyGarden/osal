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

#include <stddef.h>

namespace osal
{
inline namespace v1
{
    template<typename T>
    struct iterator final
    {
        // You need these tags to provide introspection and conform
        // to standard implementations of iterators. If you were
        // using Boost to define your iterator, you'd define those in
        // the base class.
        using value_type        = T;
        using pointer           = value_type*;
        using reference         = value_type&;

        constexpr inline explicit iterator(pointer ptr) OSAL_NOEXCEPT : ptr(ptr) {}

        constexpr inline reference operator*() const OSAL_NOEXCEPT { return *ptr; }
        constexpr inline pointer operator->() OSAL_NOEXCEPT { return ptr; }

        // Prefix increment
        constexpr inline iterator& operator++() OSAL_NOEXCEPT { ptr++; return *this; }

        // Postfix increment
        constexpr inline iterator operator++(int) OSAL_NOEXCEPT { iterator tmp = *this; ++(*this); return tmp; }

        constexpr inline friend bool operator== (const iterator& a, const iterator& b) OSAL_NOEXCEPT  { return a.ptr == b.ptr; };
        constexpr inline friend bool operator!= (const iterator& a, const iterator& b) OSAL_NOEXCEPT  { return a.ptr != b.ptr; };

    private:
        pointer ptr;
    };



}
}
