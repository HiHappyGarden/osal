/***************************************************************************
 *
 * PROJECT
 * Copyright (C) 202X  Antonio Salsi <passy.linux@zresa.it>
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


struct true_type { static constexpr inline const bool value = true; };

struct false_type { static constexpr inline const bool value = false; };

template<typename>
struct is_lvalue_reference : public false_type { };

template<typename T>
struct is_lvalue_reference<T&> : public true_type { };

template <typename T>
inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

template <class T>
constexpr T&& forward(remove_reference<T>& t) noexcept
{
    return static_cast<T&&>(t);
}

template <class T>
constexpr T&& forward(remove_reference<T>&& t) noexcept
{
    static_assert(!is_lvalue_reference_v<T>);
    return static_cast<T&&>(t);
}

template <typename T>
typename remove_reference<T>::type&& move(T&& arg)
{
    return static_cast<typename remove_reference<T>::type&&>(arg);
}

template<typename T> void swap(T& t1, T& t2) {
    T temp = move(t1);
    t1 = move(t2);
    t2 = move(temp);
}


template <typename T>
struct default_delete
{
    default_delete() = default;
    void operator()(T* ptr) const { delete ptr; }
};

template <typename T, typename Deleter = default_delete<T>>
//template <typename T, typename Deleter>
class unique_ptr {
    T* ptr = nullptr;
    Deleter deleter = Deleter();
public:
    unique_ptr() = default;
    unique_ptr(T* ptr) : ptr(ptr) {}
    unique_ptr(T* ptr, const Deleter& deleter) : ptr(ptr), deleter(deleter) {}

    inline ~unique_ptr() { deleter(ptr); }

    unique_ptr(const unique_ptr& other) = delete;

    inline unique_ptr(unique_ptr&& other) noexcept
        : ptr(other.release()),
        deleter(other.deleter)
    {}

    // generalized move ctor
    template <typename U, typename E>
    unique_ptr(unique_ptr<U, E>&& other) noexcept
        : ptr(other.release())
        , deleter(forward<E>(other.get_deleter()))
    {}

    unique_ptr& operator=(const unique_ptr& other) = delete;

    unique_ptr& operator=(unique_ptr&& other) noexcept
    {
        unique_ptr(move(other)).swap(*this);
        return *this;
    }

    void reset(T* ptr) noexcept
    {
        deleter(ptr);
        ptr = ptr;
    }

    T* release() noexcept
    {
        auto old_ptr = ptr;
        ptr = nullptr;
        return old_ptr;
    }

    void swap(unique_ptr& other) noexcept
    {
        swap(ptr, other.ptr);
    }

    T& operator*() const noexcept { return *ptr; }
    T* operator->() const noexcept { return ptr; }
    T* get() const noexcept { return ptr; }
    Deleter get_deleter() const noexcept { return deleter; }
    explicit operator bool() { return ptr != nullptr; }
};

}
}

