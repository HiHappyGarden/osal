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
#include "osal/array.hpp"


namespace osal
{
inline namespace v1
{

class value final
{
    union
    {
        char value_char;
        bool value_bool;
        char* value_str; //reference!!!!!!!
        int8_t value_int8;
        uint8_t value_uint8;
        int16_t value_int16;
        uint16_t value_uint16;
        int32_t value_int32;
        uint32_t value_uint32;
        int64_t value_int64;
        uint64_t value_uint64;
        float value_float;
        double value_double;
    };
    trait_type type;

public:
    inline value() OS_NOEXCEPT : type(trait_type::_VOID_) {}
    inline value(char value_char) OS_NOEXCEPT : value_char(value_char), type(trait_type::CHAR) {} //keep not explicit
    inline value(bool value_bool) OS_NOEXCEPT : value_bool(value_bool), type(trait_type::BOOL) {} //keep not explicit
    inline value(char* value_str) OS_NOEXCEPT : value_str(value_str), type(trait_type::STR) {} //keep not explicit
    inline value(int8_t value_int8) OS_NOEXCEPT : value_int8(value_int8), type(trait_type::INT8) {} //keep not explicit
    inline value(uint8_t value_uint8) OS_NOEXCEPT : value_uint8(value_uint8), type(trait_type::UINT8) {} //keep not explicit
    inline value(int16_t value_int16) OS_NOEXCEPT : value_int16(value_int16), type(trait_type::INT16) {} //keep not explicit
    inline value(uint16_t value_uint16) OS_NOEXCEPT : value_uint16(value_uint16), type(trait_type::UINT16) {} //keep not explicit
    inline value(int32_t value_int32) OS_NOEXCEPT : value_int32(value_int32), type(trait_type::INT32) {}  //keep not explicit
    inline value(uint32_t value_uint32) OS_NOEXCEPT : value_uint32(value_uint32), type(trait_type::UINT32) {} //keep not explicit
    inline value(int64_t value_int64) OS_NOEXCEPT : value_int64(value_int64), type(trait_type::INT64) {} //keep not explicit
    inline value(uint64_t value_uint64) OS_NOEXCEPT : value_uint64(value_uint64), type(trait_type::UINT64) {} //keep not explicit
    inline value(float value_float) OS_NOEXCEPT : value_double(value_float), type(trait_type::FLOAT) {} //keep not explicit
    inline value(double value_double) OS_NOEXCEPT : value_double(value_double), type(trait_type::DOUBLE) {} //keep not explicit

    [[nodiscard]] inline const trait_type& get_type() const OS_NOEXCEPT { return type; }

    [[nodiscard]] inline char get_char() const OS_NOEXCEPT { return value_char; }
    [[nodiscard]] inline const char* get_str() const OS_NOEXCEPT { return value_str; }
    [[nodiscard]] inline int8_t get_int8() const OS_NOEXCEPT { return value_int8; }
    [[nodiscard]] inline uint8_t get_uint8() const OS_NOEXCEPT { return value_uint8; }
    [[nodiscard]] inline int16_t get_int16() const OS_NOEXCEPT { return value_int16; }
    [[nodiscard]] inline uint16_t get_uint16() const OS_NOEXCEPT { return value_uint16; }
    [[nodiscard]] inline int32_t get_int32() const OS_NOEXCEPT { return value_int32; }
    [[nodiscard]] inline uint32_t get_uint32() const OS_NOEXCEPT { return value_uint32; }
    [[nodiscard]] inline int64_t get_int64() const OS_NOEXCEPT { return value_int64; }
    [[nodiscard]] inline uint64_t get_uint64() const OS_NOEXCEPT { return value_uint64; }
};

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
    enum type type = NONE;

    trait_type    args_type[MAX_PARAM]{trait_type::_VOID_, trait_type::_VOID_, trait_type::_VOID_};
    const uint8_t args_count   = 0;
    trait_type    ret_type     = trait_type::_VOID_;
    bool          by_reference = false;

    inline explicit function_base(enum type type, const uint8_t args_count = 0, trait_type ret_type = trait_type::_VOID_, bool by_reference = false) OS_NOEXCEPT
    : type(type), args_count(args_count), ret_type(ret_type), by_reference(by_reference)
    {
        memcpy(this->args_type, args_type, sizeof(this->args_type));
    }

public:
    virtual osal::exit execute(array<value, MAX_PARAM>&& params, char* ret_buffer, size_t ret_buffer_len) OS_NOEXCEPT = 0;

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

    osal::exit execute(array<value, MAX_PARAM>&& params, char* ret_buffer, size_t ret_buffer_len) OS_NOEXCEPT override;

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

template<typename T, typename R, typename A0, typename A1, typename A2>
osal::exit method<T, R, A0, A1, A2>::execute(array<value, MAX_PARAM>&& params, char* ret_buffer, size_t ret_buffer_len) OS_NOEXCEPT
{
    
    return exit::KO;
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

    osal::exit execute(array<value, MAX_PARAM>&& params, char* ret_buffer, size_t ret_buffer_len) OS_NOEXCEPT override;
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

template<typename R, typename A0, typename A1, typename A2>
osal::exit function<R, A0, A1, A2>::execute(array<value, MAX_PARAM>&& params, char* ret_buffer, size_t ret_buffer_len) OS_NOEXCEPT
{
    return exit::KO;
}


}
}