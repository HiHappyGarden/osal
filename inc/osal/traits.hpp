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

enum class trait_type : uint8_t
{
    CHAR,
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
struct is_type
{
    constexpr static const trait_type type = trait_type::CUSTOM;
};

template<>
struct is_type<char>
{
    constexpr static const trait_type type = trait_type::CHAR;
};

template<>
struct is_type<char *>
{
    constexpr static const trait_type type = trait_type::STR;
};

template<>
struct is_type<const char *>
{
    constexpr static const trait_type type = trait_type::STR;
};

template<size_t Size>
struct is_type<osal::string<Size>>
{
    constexpr static const trait_type type = trait_type::STRING;
};

template<size_t Size>
struct is_type<osal::string<Size>&>
{
    constexpr static const trait_type type = trait_type::STRING;
};

template<size_t Size>
struct is_type<const osal::string<Size>&>
{
    constexpr static const trait_type type = trait_type::STRING;
};

template<size_t Size>
struct is_type<osal::string<Size>&&>
{
    constexpr static const trait_type type = trait_type::STRING;
};

template<>
struct is_type<int8_t>
{
    constexpr static const trait_type type = trait_type::INT8;
};

template<>
struct is_type<uint8_t>
{
    constexpr static const trait_type type = trait_type::UINT8;
};

template<>
struct is_type<int16_t>
{
    constexpr static const trait_type type = trait_type::INT16;
};

template<>
struct is_type<uint16_t>
{
    constexpr static const trait_type type = trait_type::UINT16;
};


template<>
struct is_type<int32_t>
{
    constexpr static const trait_type type = trait_type::INT32;
};

template<>
struct is_type<uint32_t>
{
    constexpr static const trait_type type = trait_type::UINT32;
};

template<>
struct is_type<int64_t>
{
    constexpr static const trait_type type = trait_type::INT64;
};

template<>
struct is_type<uint64_t>
{
    constexpr static const trait_type type = trait_type::UINT64;
};

template<>
struct is_type<float>
{
    constexpr static const trait_type type = trait_type::FLOAT;
};

template<>
struct is_type<double>
{
    constexpr static const trait_type type = trait_type::DOUBLE;
};

template<typename ... Args>
class parameter_helper final
{
    trait_type parameters[sizeof...(Args)];
    uint8_t parameters_size = 0;

public:
    inline constexpr parameter_helper() OS_NOEXCEPT
    {
        //getter(Args...);
    }

    inline constexpr explicit parameter_helper(Args ... args) OS_NOEXCEPT
    {
        getter(args...);
    }

    template<typename T>
    inline constexpr void getter(T t, Args ... args) OS_NOEXCEPT
    {
        parameters[parameters_size] = is_type(t);
        parameters_size++;
        getter(args...);
    }

    template<typename T>
    inline constexpr void getter(T t) OS_NOEXCEPT
    {
        parameters[parameters_size] = is_type(t);
        parameters_size++;
    }

    inline constexpr uint8_t size() OS_NOEXCEPT
    {
        return parameters_size;
    }

    inline constexpr const trait_type* get_parameters() OS_NOEXCEPT
    {
        return parameters;
    }
};

template<typename TClass, typename TResult, typename ... Type>
struct smart_call {
    // call for non-const class methods
    static TResult invoke(TClass* context, TResult (TClass::*method)(Type...), Type... args)
    {
        return (context->*(method))(args ...);
    }

    // call for const class methods
    static TResult invoke(TClass* context, TResult (TClass::*method)(Type...) const, Type... args)
    {
        return (context->*(method))(args ...);
    }
};

// SmartCall specialization for 'void' return type
template<typename TClass, typename ... Type>
struct smart_call<TClass, void, Type...> {
    static void invoke(TClass* context, void (TClass::*method)(Type...), Type... args)
    {
        (context->*(method))(args ...);
    }
    static void invoke(TClass* context, void(TClass::*method)(Type...) const, Type... args)
    {
        return (context->*(method))(args ...);
    }
};


}
}

