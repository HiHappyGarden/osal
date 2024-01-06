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

template<typename>
struct is_lvalue_reference : public false_type { };

template<typename T>
struct is_lvalue_reference<T&> : public true_type { };

template <typename T>
inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

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

