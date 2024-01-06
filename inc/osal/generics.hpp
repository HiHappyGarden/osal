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

#include "osal/traits.hpp"
#include "osal/types.hpp"
#include "osal/memory.hpp"


namespace osal
{
inline namespace v1
{

struct function_base
{
    using ptr = unique_ptr<function_base>;

    static constexpr const uint8_t MAX_PARAM = 3;

    enum type
    {
        NONE, METHOD, FUNCTION
    };

    function_base() = default;

    [[nodiscard]] inline uint8_t get_args_count() const OS_NOEXCEPT
    {
        return args_count;
    }

    [[nodiscard]] inline const trait_type* get_args_type() const OS_NOEXCEPT
    {
        return args_type;
    }

    [[nodiscard]] inline trait_type get_ret_type() const OS_NOEXCEPT
    {
        return ret_type;
    }

    [[nodiscard]] inline bool get_by_reference() const OS_NOEXCEPT
    {
        return by_reference;
    }

    [[nodiscard]] inline type get_type() const OS_NOEXCEPT
    {
        return type;
    }

protected:
    inline explicit function_base(enum type type, const uint8_t args_count = 0, trait_type ret_type = trait_type::_VOID_, bool by_reference = false) OS_NOEXCEPT
            : type(type), args_count(args_count), ret_type(ret_type), by_reference(by_reference)
    {
        memcpy(this->args_type, args_type, sizeof(this->args_type));
    }

    enum type type = NONE;

    trait_type    args_type[MAX_PARAM]{trait_type::_VOID_, trait_type::_VOID_, trait_type::_VOID_};
    const uint8_t args_count   = 0;
    trait_type    ret_type     = trait_type::_VOID_;
    bool          by_reference = false;
};

template<typename T, typename R, typename A0 = no_class, typename A1 = no_class, typename A2 = no_class>
class method final : public function_base
{

    T* target = nullptr;

    union method_prt
    {
        R (T::*method_no_arg)();

        R (T::*method_a0)(A0);

        R (T::*method_ref_a0)(const A0&);

        R (T::*method_a0_a1)(A0, A1);

        R (T::*method_ref_a0_a1)(const A0&, const A1&);

        R (T::*method_a0_a1_a2)(A0, A1, A2);

        R (T::*method_ref_a0_a1_a2)(const A0&, const A1&, const A2&);
    } method_prt{nullptr};


public:
    method(T* target, R (T::*method)(), trait_type ret_type) OS_NOEXCEPT;

    method(T* target, R (T::*method)(A0), trait_type ret_type, trait_type type0) OS_NOEXCEPT;

    method(T* target, R (T::*method)(const A0&), trait_type ret_type, trait_type type0) OS_NOEXCEPT;

    method(T* target, R (T::*method)(A0, A1), trait_type ret_type, trait_type type0, trait_type type1) OS_NOEXCEPT;

    method(T* target, R (T::*method)(const A0&, const A1&), trait_type ret_type, trait_type type0, trait_type type1) OS_NOEXCEPT;

    method(T* target, R (T::*method)(A0, A1, A2), trait_type ret_type, trait_type type0, trait_type type1, trait_type type2) OS_NOEXCEPT;

    method(T* target, R (T::*method)(const A0&, const A1&, const A2&), trait_type ret_type, trait_type type0, trait_type type1, trait_type type2) OS_NOEXCEPT;

    method(const method&) = delete;

    method& operator=(const method&) = delete;

    method(method&&) = delete;

    method& operator=(method&&) = delete;

    [[nodiscard]] inline T* get_target() const OS_NOEXCEPT
    {
        return target;
    }

    [[nodiscard]] inline const union method_prt& get_method() const OS_NOEXCEPT
    {
        return method_prt;
    }


};

template<typename T, typename R, typename A0, typename A1, typename A2>
method<T, R, A0, A1, A2>::method(T* target, R (T::*method)(), trait_type ret_type) OS_NOEXCEPT
: function_base(METHOD, 0, ret_type), target(target)
{
    method_prt.method_no_arg = method;
}

template<typename T, typename R, typename A0, typename A1, typename A2>
method<T, R, A0, A1, A2>::method(T* target, R (T::*method)(A0), trait_type ret_type, trait_type type0) OS_NOEXCEPT
: function_base(METHOD, 1, ret_type), target(target)
{
    args_type[0] = type0;
    method_prt.method_a0 = method;
}

template<typename T, typename R, typename A0, typename A1, typename A2>
method<T, R, A0, A1, A2>::method(T* target, R (T::*method)(const A0&), trait_type ret_type, trait_type type0) OS_NOEXCEPT
: function_base(METHOD, 1, ret_type, true), target(target)
{
    args_type[0] = type0;
    method_prt.method_ref_a0 = method;
}

template<typename T, typename R, typename A0, typename A1, typename A2>
method<T, R, A0, A1, A2>::method(T* target, R (T::*method)(A0, A1), trait_type ret_type, trait_type type0, trait_type type1) OS_NOEXCEPT
        : function_base(METHOD, 2, ret_type), target(target)
{
    args_type[0] = type0;
    args_type[1] = type1;
    method_prt.method_a0_a1 = method;
}

template<typename T, typename R, typename A0, typename A1, typename A2>
method<T, R, A0, A1, A2>::method(T* target, R (T::*method)(const A0&, const A1&), trait_type ret_type, trait_type type0, trait_type type1) OS_NOEXCEPT
: function_base(METHOD, 2, ret_type, true), target(target)
{
    args_type[0] = type0;
    args_type[1] = type1;
    method_prt.method_ref_a0_a1 = method;
}

template<typename T, typename R, typename A0, typename A1, typename A2>
method<T, R, A0, A1, A2>::method(T* target, R (T::*method)(A0, A1, A2), trait_type ret_type, trait_type type0, trait_type type1, trait_type type2) OS_NOEXCEPT
: function_base(METHOD, 3, ret_type), target(target)
{
    args_type[0] = type0;
    args_type[1] = type1;
    args_type[2] = type2;
    method_prt.method_a0_a1_a2 = method;
}

template<typename T, typename R, typename A0, typename A1, typename A2>
method<T, R, A0, A1, A2>::method(T* target, R (T::*method)(const A0&, const A1&, const A2&), trait_type ret_type, trait_type type0, trait_type type1, trait_type type2) OS_NOEXCEPT
: function_base(METHOD, 3, ret_type, true), target(target)
{
    args_type[0] = type0;
    args_type[1] = type1;
    args_type[2] = type2;
    method_prt.method_ref_a0_a1_a2 = method;
}

template<typename R, typename A0 = no_class, typename A1 = no_class, typename A2 = no_class>
class function final : public function_base
{
    union function_prt
    {
        R (* function_no_arg)();

        R (* function_a0)(A0);

        R (* function_ref_a0)(const A0&);

        R (* function_a0_a1)(A0, A1);

        R (* function_ref_a0_a1)(const A0&, const A1&);

        R (* function_a0_a1_a2)(A0, A1, A2);

        R (* function_ref_a0_a1_a2)(const A0&, const A1&, const A2&);
    } function_prt{nullptr};

public:
    explicit function(R (* function)(), trait_type ret_type) OS_NOEXCEPT;

    explicit function(R (* function)(A0), trait_type ret_type, trait_type type0) OS_NOEXCEPT;

    explicit function(R (* function)(const A0&), trait_type ret_type, trait_type type0) OS_NOEXCEPT;

    explicit function(R (* function)(A0, A1), trait_type ret_type, trait_type type0, trait_type type1) OS_NOEXCEPT;

    explicit function(R (* function)(const A0&, const A1&), trait_type ret_type, trait_type type0, trait_type type1) OS_NOEXCEPT;

    explicit function(R (* function)(A0, A1, A2), trait_type ret_type, trait_type type0, trait_type type1, trait_type type2) OS_NOEXCEPT;

    explicit function(R (* function)(const A0&, const A1&, const A2&), trait_type ret_type, trait_type type0, trait_type type1, trait_type type2) OS_NOEXCEPT;

    function(const function&) = delete;

    function& operator=(const function&) = delete;

    function(function&&) = delete;

    function& operator=(function&&) = delete;

    [[nodiscard]] inline const union function_prt& get_function() const OS_NOEXCEPT
    {
        return function_prt;
    }

};

template<typename R, typename A0, typename A1, typename A2>
function<R, A0, A1, A2>::function(R (* function)(), trait_type ret_type) OS_NOEXCEPT
: function_base(FUNCTION, 0, ret_type)
{
    function_prt.function_no_arg = function;
}

template<typename R, typename A0, typename A1, typename A2>
function<R, A0, A1, A2>::function(R (* function)(A0), trait_type ret_type, trait_type type0) OS_NOEXCEPT
: function_base(FUNCTION, 1, ret_type)
{
    args_type[0] = type0;
    function_prt.function_a0 = function;
}

template<typename R, typename A0, typename A1, typename A2>
function<R, A0, A1, A2>::function(R (* function)(const A0&), trait_type ret_type, trait_type type0) OS_NOEXCEPT
: function_base(FUNCTION, 1, ret_type, true)
{
    args_type[0] = type0;
    function_prt.function_ref_a0 = function;
}

template<typename R, typename A0, typename A1, typename A2>
function<R, A0, A1, A2>::function(R (* function)(A0, A1), trait_type ret_type, trait_type type0, trait_type type1) OS_NOEXCEPT
: function_base(FUNCTION, 2, ret_type)
{
    args_type[0] = type0;
    args_type[1] = type1;
    function_prt.function_a0_a1 = function;
}

template<typename R, typename A0, typename A1, typename A2>
function<R, A0, A1, A2>::function(R (* function)(const A0&, const A1&), trait_type ret_type, trait_type type0, trait_type type1) OS_NOEXCEPT
: function_base(FUNCTION, 2, ret_type, true)
{
    args_type[0] = type0;
    args_type[1] = type1;
    function_prt.function_ref_a0_a1 = function;
}

template<typename R, typename A0, typename A1, typename A2>
function<R, A0, A1, A2>::function(R (* function)(A0, A1, A2), trait_type ret_type, trait_type type0, trait_type type1, trait_type type2) OS_NOEXCEPT
: function_base(FUNCTION, 3, ret_type)
{
    args_type[0] = type0;
    args_type[1] = type1;
    args_type[2] = type2;
    function_prt.function_a0_a1_a2 = function;
}

template<typename R, typename A0, typename A1, typename A2>
function<R, A0, A1, A2>::function(R (* function)(const A0&, const A1&, const A2&), trait_type ret_type, trait_type type0, trait_type type1, trait_type type2) OS_NOEXCEPT
: function_base(FUNCTION, 3, ret_type, true)
{
    args_type[0] = type0;
    args_type[1] = type1;
    args_type[2] = type2;
    function_prt.function_ref_a0_a1_a2 = function;
}

}
}