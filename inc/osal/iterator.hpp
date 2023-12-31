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
class iterator final
{
public:
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    iterator() : m_ptr(nullptr) {}
    iterator(pointer ptr) : m_ptr(ptr) {}

    reference operator*() const { return *m_ptr; }
    pointer operator->() const { return m_ptr; }
    reference operator[](difference_type n) const { return *(m_ptr + n); }

    iterator& operator++() {
        ++m_ptr;
        return *this;
    }
    iterator operator++(int) {
        iterator temp = *this;
        ++m_ptr;
        return temp;
    }
    iterator& operator--() {
        --m_ptr;
        return *this;
    }
    iterator operator--(int) {
        iterator temp = *this;
        --m_ptr;
        return temp;
    }
    iterator& operator+=(difference_type n) {
        m_ptr += n;
        return *this;
    }
    iterator& operator-=(difference_type n) {
        m_ptr -= n;
        return *this;
    }

    friend bool operator==(const iterator& lhs, const iterator& rhs) {
        return lhs.m_ptr == rhs.m_ptr;
    }
    friend bool operator!=(const iterator& lhs, const iterator& rhs) {
        return !(lhs == rhs);
    }
    friend bool operator<(const iterator& lhs, const iterator& rhs) {
        return lhs.m_ptr < rhs.m_ptr;
    }
    friend bool operator>(const iterator& lhs, const iterator& rhs) {
        return rhs < lhs;
    }
    friend bool operator<=(const iterator& lhs, const iterator& rhs) {
        return !(rhs < lhs);
    }
    friend bool operator>=(const iterator& lhs, const iterator& rhs) {
        return !(lhs < rhs);
    }
    friend iterator operator+(const iterator& it, difference_type n) {
        iterator temp = it;
        temp += n;
        return temp;
    }
    friend iterator operator+(difference_type n, const iterator& it) {
        return it + n;
    }
    friend iterator operator-(const iterator& it, difference_type n) {
        iterator temp = it;
        temp -= n;
        return temp;
    }
    friend difference_type operator-(const iterator& lhs, const iterator& rhs) {
        return lhs.m_ptr - rhs.m_ptr;
    }

private:
    pointer m_ptr;
};



}
}
