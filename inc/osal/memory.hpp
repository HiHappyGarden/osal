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

#include "osal/traits.hpp"

#include <stdlib.h>

namespace osal
{
inline namespace v1
{

template <class T>
struct remove_reference { typedef T type; };

template <class T>
struct remove_reference<T&> { typedef T type; };

template <class T>
struct remove_reference<T&&> { typedef T type; };

template <typename T>
inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

template <typename T>
typename remove_reference<T>::type&& move(T&& arg) OS_NOEXCEPT
{
    return static_cast<typename remove_reference<T>::type&&>(arg);
}

template <class T>
constexpr T&& forward(remove_reference<T>& t) OS_NOEXCEPT
{
    return static_cast<T&&>(t);
}

template <class T>
constexpr T&& forward(remove_reference<T>&& t) OS_NOEXCEPT
{
    static_assert(!is_lvalue_reference_v<T>);
    return static_cast<T&&>(t);
}


template <typename T>
struct default_delete
{
    default_delete() = default;
    inline void operator()(T* ptr) const OS_NOEXCEPT
    {
        delete ptr;
    }
};

template<typename T>
struct array_deleter
{
    void operator()(T* ptr) const OS_NOEXCEPT
    {
        delete [] ptr;
    }
};


template <typename T, typename Deleter = default_delete<T>>
class unique_ptr 
{
    T* ptr = nullptr;
    Deleter deleter = Deleter();
public:
    unique_ptr() = default;
    unique_ptr(T* ptr) OS_NOEXCEPT : ptr(ptr) {}
    unique_ptr(T* ptr, const Deleter& deleter) OS_NOEXCEPT : ptr(ptr), deleter(deleter) {}

    inline ~unique_ptr()
    {
        deleter(ptr);
        ptr = nullptr;
    }

    unique_ptr(const unique_ptr& other) = delete;

    inline unique_ptr(unique_ptr&& other) OS_NOEXCEPT
        : ptr(other.release()),
        deleter(other.deleter)
    {}

    // generalized move ctor
    template <typename U, typename E>
    unique_ptr(unique_ptr<U, E>&& other) OS_NOEXCEPT
        : ptr(other.release())
        , deleter(forward<E>(other.get_deleter()))
    {}

    unique_ptr& operator=(const unique_ptr& other) = delete;

    unique_ptr& operator=(unique_ptr&& other) OS_NOEXCEPT
    {
        unique_ptr(move(other)).swap(*this);
        return *this;
    }

    void reset(T* ptr) OS_NOEXCEPT
    {
        deleter(ptr);
        ptr = ptr;
    }

    T* release() OS_NOEXCEPT
    {
        auto old_ptr = ptr;
        ptr = nullptr;
        return old_ptr;
    }

    inline void swap(unique_ptr& other) OS_NOEXCEPT
    {
        T* temp = ptr;
        ptr = other.ptr;
        other.ptr = temp;
    }

    inline bool is_null() OS_NOEXCEPT { return ptr == nullptr; }
    //inline ssize_t size() OS_NOEXCEPT { return sizeof(T); }
    inline T& operator[](size_t idx) OS_NOEXCEPT { return ptr[idx]; }
    inline const T& operator[](size_t idx) const OS_NOEXCEPT { return ptr[idx]; }
    inline T& operator*() const OS_NOEXCEPT { return *ptr; }
    inline T* operator->() const OS_NOEXCEPT { return ptr; }
    inline T* get() const OS_NOEXCEPT { return ptr; }
    inline Deleter get_deleter() const OS_NOEXCEPT { return deleter; }
    inline explicit operator bool() OS_NOEXCEPT { return ptr != nullptr; }

};

extern array_deleter<bool> array_deleter_bool;
extern array_deleter<char> array_deleter_char;
extern array_deleter<short> array_deleter_short;
extern array_deleter<int> array_deleter_int;
extern array_deleter<long> array_deleter_long;
extern array_deleter<float> array_deleter_float;
extern array_deleter<double> array_deleter_double;
extern array_deleter<int8_t> array_deleter_uint8;
extern array_deleter<uint8_t> array_deleter_int8;
extern array_deleter<int16_t> array_deleter_uint16;
extern array_deleter<uint16_t> array_deleter_int16;
extern array_deleter<int32_t> array_deleter_uint32;
extern array_deleter<uint32_t> array_deleter_int32;
extern array_deleter<int64_t> array_deleter_uint64;
extern array_deleter<uint64_t> array_deleter_int64;

}
}

