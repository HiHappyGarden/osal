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
#include "osal/types.hpp"
#include "osal/string.hpp"

#include <stdio.h>
#include <stdlib.h>

namespace osal
{
inline namespace v1
{

constexpr const uint8_t MAX_PARAM = 3;

template<typename>
struct is_lvalue_reference : public false_type { };

template<typename T>
struct is_lvalue_reference<T&> : public true_type { };

struct no_class final
{
    constexpr no_class() = default;
};

enum class trait_type : uint8_t
{
    _VOID_,
    CHAR,
    BOOL,
    STR,
    STRING,
    INT8,
    UINT8,
    INT16,
    UINT16,
    INT32,
    UINT32,
    INT64,
    UINT64,
    FLOAT,
    DOUBLE,
    CUSTOM
};

template<typename T>
struct get_type
{
    constexpr static const trait_type type = trait_type::CUSTOM;
};

template<>
struct get_type<void>
{
    constexpr static const trait_type type = trait_type::_VOID_;
};

//template<>
//struct get_type<no_class>
//{
//    constexpr static const trait_type type = trait_type::_VOID_;
//};

template<>
struct get_type<char>
{
    constexpr static const trait_type type = trait_type::CHAR;
};

template<>
struct get_type<char *>
{
    constexpr static const trait_type type = trait_type::STR;
};

template<>
struct get_type<const char *>
{
    constexpr static const trait_type type = trait_type::STR;
};

template<size_t Size>
struct get_type<osal::string<Size>>
{
    constexpr static const trait_type type = trait_type::STRING;
};

template<size_t Size>
struct get_type<osal::string<Size>&>
{
    constexpr static const trait_type type = trait_type::STRING;
};

template<size_t Size>
struct get_type<const osal::string<Size>&>
{
    constexpr static const trait_type type = trait_type::STRING;
};

template<size_t Size>
struct get_type<osal::string<Size>&&>
{
    constexpr static const trait_type type = trait_type::STRING;
};

template<>
struct get_type<bool>
{
    constexpr static const trait_type type = trait_type::BOOL;
};

template<>
struct get_type<int8_t>
{
    constexpr static const trait_type type = trait_type::INT8;
};

template<>
struct get_type<uint8_t>
{
    constexpr static const trait_type type = trait_type::UINT8;
};

template<>
struct get_type<int16_t>
{
    constexpr static const trait_type type = trait_type::INT16;
};

template<>
struct get_type<uint16_t>
{
    constexpr static const trait_type type = trait_type::UINT16;
};


template<>
struct get_type<int32_t>
{
    constexpr static const trait_type type = trait_type::INT32;
};

template<>
struct get_type<uint32_t>
{
    constexpr static const trait_type type = trait_type::UINT32;
};

template<>
struct get_type<int64_t>
{
    constexpr static const trait_type type = trait_type::INT64;
};

template<>
struct get_type<uint64_t>
{
    constexpr static const trait_type type = trait_type::UINT64;
};

template<>
struct get_type<float>
{
    constexpr static const trait_type type = trait_type::FLOAT;
};

template<>
struct get_type<double>
{
    constexpr static const trait_type type = trait_type::DOUBLE;
};

template<typename T, typename R, typename ... Ts>
struct smart_call
{
    // call for non-const class methods
    static R invoke(T* context, R (T::*method)(Ts...), Ts... args)
    {
        return (context->*(method))(args ...);
    }

    // call for const class methods
    static R invoke(T* context, R (T::*method)(Ts...) const, Ts... args)
    {
        return (context->*(method))(args ...);
    }
};

// SmartCall specialization for 'void' return type
template<typename T, typename ... Ts>
struct smart_call<T, void, Ts...>
{
    static void invoke(T* context, void (T::*method)(Ts...), Ts... args)
    {
        (context->*(method))(args ...);
    }
    static void invoke(T* context, void(T::*method)(Ts...) const, Ts... args)
    {
        return (context->*(method))(args ...);
    }
};

struct function_base
{
    enum type
    {
        NONE,
        METHOD,
        FUNCTION
    };

    function_base() = default;

    [[nodiscard]] inline uint8_t get_args_count() const OS_NOEXCEPT
    {
        return args_count;
    }

    [[nodiscard]] inline const trait_type*  get_args_type() const OS_NOEXCEPT
    {
        return args_type;
    }

    [[nodiscard]] inline trait_type  get_ret_type() const OS_NOEXCEPT
    {
        return ret_type;
    }

    [[nodiscard]] inline bool  get_by_reference() const OS_NOEXCEPT
    {
        return by_reference;
    }

    [[nodiscard]] inline type get_type() const OS_NOEXCEPT
    {
        return type;
    }

protected:
    inline explicit function_base(enum type type, const uint8_t args_count = 0, trait_type ret_type = trait_type::_VOID_, bool by_reference = false) OS_NOEXCEPT : type(type), args_count(args_count), ret_type(ret_type), by_reference(by_reference)
    {
        memcpy(this->args_type, args_type, sizeof(this->args_type));
    }

    enum type type = NONE;

    trait_type args_type[MAX_PARAM] {trait_type::_VOID_, trait_type::_VOID_, trait_type::_VOID_};
    const uint8_t args_count = 0;
    trait_type ret_type = trait_type::_VOID_;
    bool by_reference = false;
};

template <typename T, typename V, typename R,
        typename A0 = no_class, typename A1 = no_class, typename A2 = no_class>
class method final : public function_base
{
    
    T* target = nullptr;
    union method_prt
    {
        R (V::*method_no_arg)();
        R (V::*method_a0)(A0);
        R (V::*method_ref_a0)(const A0&);
        R (V::*method_a0_a1)(A0, A1);
        R (V::*method_ref_a0_a1)(const A0&, const A1&);
        R (V::*method_a0_a1_a2)(A0, A1, A2);
        R (V::*method_ref_a0_a1_a2)(const A0&, const A1&, const A2&);
    } method_prt {nullptr};


public:
    method(T *target, R (V::*method)(), trait_type ret_type) OS_NOEXCEPT;
    method(T *target, R (V::*method)(A0), trait_type ret_type, trait_type type0) OS_NOEXCEPT;
    method(T *target, R (V::*method)(const A0&), trait_type ret_type, trait_type type0) OS_NOEXCEPT;
    method(T *target, R (V::*method)(A0, A1), trait_type ret_type, trait_type type0, trait_type type1) OS_NOEXCEPT;
    method(T *target, R (V::*method)(const A0&, const A1&), trait_type ret_type, trait_type type0, trait_type type1) OS_NOEXCEPT;
    method(T *target, R (V::*method)(A0, A1, A2), trait_type ret_type, trait_type type0, trait_type type1, trait_type type2) OS_NOEXCEPT;
    method(T *target, R (V::*method)(const A0&, const A1&, const A2&), trait_type ret_type, trait_type type0, trait_type type1, trait_type type2) OS_NOEXCEPT;
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

template<typename T, typename V, typename R, typename A0, typename A1, typename A2>
method<T, V, R, A0, A1, A2>::method(T* target, R (V::*method)(), trait_type ret_type) OS_NOEXCEPT
: function_base(METHOD, 0, ret_type)
, target(target)
{
    method_prt.method_no_arg = method;
}

template<typename T, typename V, typename R, typename A0, typename A1, typename A2>
method<T, V, R, A0, A1, A2>::method(T* target, R (V::*method)(A0), trait_type ret_type, trait_type type0) OS_NOEXCEPT
: function_base(METHOD, 1, ret_type)
, target(target)
{
    args_type[0] = type0;
    method_prt.method_a0 = method;
}

template<typename T, typename V, typename R, typename A0, typename A1, typename A2>
method<T, V, R, A0, A1, A2>::method(T* target, R (V::*method)(const A0&), trait_type ret_type, trait_type type0) OS_NOEXCEPT
: function_base(METHOD, 1, ret_type, true)
, target(target)
{
    args_type[0] = type0;
    method_prt.method_ref_a0 = method;
}

template<typename T, typename V, typename R, typename A0, typename A1, typename A2>
method<T, V, R, A0, A1, A2>::method(T* target, R (V::*method)(A0, A1), trait_type ret_type, trait_type type0, trait_type type1) OS_NOEXCEPT
: function_base(METHOD, 2, ret_type)
, target(target)
{
    args_type[0] = type0;
    args_type[1] = type1;
    method_prt.method_a0_a1 = method;
}

template<typename T, typename V, typename R, typename A0, typename A1, typename A2>
method<T, V, R, A0, A1, A2>::method(T* target, R (V::*method)(const A0&, const A1&), trait_type ret_type, trait_type type0, trait_type type1) OS_NOEXCEPT
: function_base(METHOD, 2, ret_type, true)
, target(target)
{
    args_type[0] = type0;
    args_type[1] = type1;
    method_prt.method_ref_a0_a1 = method;
}

template<typename T, typename V, typename R, typename A0, typename A1, typename A2>
method<T, V, R, A0, A1, A2>::method(T* target, R (V::*method)(A0, A1, A2), trait_type ret_type, trait_type type0, trait_type type1, trait_type type2) OS_NOEXCEPT
: function_base(METHOD, 3, ret_type)
, target(target)
{
    args_type[0] = type0;
    args_type[1] = type1;
    args_type[2] = type2;
    method_prt.method_a0_a1_a2 = method;
}

template<typename T, typename V, typename R, typename A0, typename A1, typename A2>
method<T, V, R, A0, A1, A2>::method(T* target, R (V::*method)(const A0&, const A1&, const A2&), trait_type ret_type, trait_type type0, trait_type type1, trait_type type2) OS_NOEXCEPT
: function_base(METHOD, 3, ret_type, true)
, target(target)
{
    args_type[0] = type0;
    args_type[1] = type1;
    args_type[2] = type2;
    method_prt.method_ref_a0_a1_a2 = method;
}

template <typename R,
        typename A0 = no_class, typename A1 = no_class, typename A2 = no_class>
class function final : public function_base
{
    union function_prt
    {
        R (*function_no_arg)();
        R (*function_a0)(A0);
        R (*function_ref_a0)(const A0&);
        R (*function_a0_a1)(A0, A1);
        R (*function_ref_a0_a1)(const A0&, const A1&);
        R (*function_a0_a1_a2)(A0, A1, A2);
        R (*function_ref_a0_a1_a2)(const A0&, const A1&, const A2&);
    } function_prt {nullptr};
public:
    explicit function(R (*function)(), trait_type ret_type) OS_NOEXCEPT;
    explicit function(R (*function)(A0), trait_type ret_type, trait_type type0) OS_NOEXCEPT;
    explicit function(R (*function)(const A0&), trait_type ret_type, trait_type type0) OS_NOEXCEPT;
    explicit function(R (*function)(A0, A1), trait_type ret_type, trait_type type0, trait_type type1) OS_NOEXCEPT;
    explicit function(R (*function)(const A0&, const A1&), trait_type ret_type, trait_type type0, trait_type type1) OS_NOEXCEPT;
    explicit function(R (*function)(A0, A1, A2), trait_type ret_type, trait_type type0, trait_type type1, trait_type type2) OS_NOEXCEPT;
    explicit function(R (*function)(const A0&, const A1&, const A2&), trait_type ret_type, trait_type type0, trait_type type1, trait_type type2) OS_NOEXCEPT;
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
function<R, A0, A1, A2>::function( R (*function)(), trait_type ret_type) OS_NOEXCEPT
: function_base(FUNCTION, 0, ret_type)
{
    function_prt.function_no_arg = function;
}

template<typename R, typename A0, typename A1, typename A2>
function<R, A0, A1, A2>::function( R (*function)(A0), trait_type ret_type, trait_type type0) OS_NOEXCEPT
: function_base(FUNCTION, 1, ret_type)
{
    args_type[0] = type0;
    function_prt.function_a0 = function;
}

template<typename R, typename A0, typename A1, typename A2>
function<R, A0, A1, A2>::function( R (*function)(const A0&), trait_type ret_type, trait_type type0) OS_NOEXCEPT
: function_base(FUNCTION, 1, ret_type, true)
{
    args_type[0] = type0;
    function_prt.function_ref_a0 = function;
}

template<typename R, typename A0, typename A1, typename A2>
function<R, A0, A1, A2>::function( R (*function)(A0, A1), trait_type ret_type, trait_type type0, trait_type type1) OS_NOEXCEPT
: function_base(FUNCTION, 2, ret_type)
{
    args_type[0] = type0;
    args_type[1] = type1;
    function_prt.function_a0_a1 = function;
}

template<typename R, typename A0, typename A1, typename A2>
function<R, A0, A1, A2>::function( R (*function)(const A0&, const A1&), trait_type ret_type, trait_type type0, trait_type type1) OS_NOEXCEPT
: function_base(FUNCTION, 2, ret_type, true)
{
    args_type[0] = type0;
    args_type[1] = type1;
    function_prt.function_ref_a0_a1 = function;
}

template<typename R, typename A0, typename A1, typename A2>
function<R, A0, A1, A2>::function( R (*function)(A0, A1, A2), trait_type ret_type, trait_type type0, trait_type type1, trait_type type2) OS_NOEXCEPT
: function_base(FUNCTION, 3, ret_type)
{
    args_type[0] = type0;
    args_type[1] = type1;
    args_type[2] = type2;
    function_prt.function_a0_a1_a2 = function;
}

template<typename R, typename A0, typename A1, typename A2>
function<R, A0, A1, A2>::function( R (*function)(const A0&, const A1&, const A2&), trait_type ret_type, trait_type type0, trait_type type1, trait_type type2) OS_NOEXCEPT
        : function_base(FUNCTION, 3, ret_type, true)
{
    args_type[0] = type0;
    args_type[1] = type1;
    args_type[2] = type2;
    function_prt.function_ref_a0_a1_a2 = function;
}

//template<typename ... Args>
//class parameter_helper final
//{
//    trait_type parameters[sizeof...(Args)];
//    uint8_t parameters_size = 0;
//
//public:
//    inline constexpr parameter_helper() OS_NOEXCEPT
//    {
//        //getter(Args...);
//    }
//
//    inline constexpr explicit parameter_helper(Args ... args) OS_NOEXCEPT
//    {
//        getter(args...);
//    }
//
//    template<typename T>
//    inline constexpr void getter(T t, Args ... args) OS_NOEXCEPT
//    {
//        parameters[parameters_size] = get_type(t);
//        parameters_size++;
//        getter(args...);
//    }
//
//    template<typename T>
//    inline constexpr void getter(T t) OS_NOEXCEPT
//    {
//        parameters[parameters_size] = get_type(t);
//        parameters_size++;
//    }
//
//    inline constexpr uint8_t size() OS_NOEXCEPT
//    {
//        return parameters_size;
//    }
//
//    inline constexpr const trait_type* get_parameters() OS_NOEXCEPT
//    {
//        return parameters;
//    }
//};


}
}

