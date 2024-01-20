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
        void* ptr;
    };
    size_t ptr_size;
    trait_type type;

public:
    inline value() OS_NOEXCEPT : type(trait_type::_VOID_) {}
    inline value(void* ptr, size_t ptr_size) OS_NOEXCEPT : ptr(ptr), ptr_size(ptr_size), type(trait_type::DOUBLE) {} //keep not explicit

    template<typename T>
    constexpr value(T t) //keep not explicit
    {
        if constexpr (is_same<void, T>::value)
        {
            type = trait_type::_VOID_;
        }
        else if constexpr (is_same<char, T>::value)
        {
            type = trait_type::CHAR;
            value_char = t;
        }
        else if constexpr (is_same<bool, T>::value)
        {
            type = trait_type::BOOL;
            value_bool = t;
        }
        else if constexpr (is_same<char*, T>::value)
        {
            type = trait_type::STR;
            value_str = t;
        }
        else if constexpr (is_same<const char*, T>::value)
        {
            type = trait_type::STR;
            value_str = const_cast<char*>(t);
        }
        else if constexpr (is_same<int8_t, T>::value)
        {
            type = trait_type::INT8;
            value_int8 = t;
        }
        else if constexpr (is_same<uint8_t, T>::value)
        {
            type = trait_type::UINT8;
            value_uint8 = t;
        }
        else if constexpr (is_same<int16_t, T>::value)
        {
            type = trait_type::INT16;
            value_int16 = t;
        }
        else if constexpr (is_same<uint16_t, T>::value)
        {
            type = trait_type::UINT16;
            value_uint16 = t;
        }
        else if constexpr (is_same<int32_t, T>::value)
        {
            type = trait_type::INT32;
            value_int32 = t;
        }
        else if constexpr (is_same<uint32_t, T>::value)
        {
            type = trait_type::UINT32;
            value_uint32 = t;
        }
        else if constexpr (is_same<int64_t, T>::value)
        {
            type = trait_type::INT64;
            value_int64 = t;
        }
        else if constexpr (is_same<uint64_t, T>::value)
        {
            type = trait_type::UINT64;
            value_uint64 = t;
        }
        else if constexpr (is_same<float, T>::value)
        {
            type = trait_type::FLOAT;
            value_float = t;
        }
        else if constexpr (is_same<double, T>::value)
        {
            type = trait_type::DOUBLE;
            value_double = t;
        }
        else
        {
            type = trait_type::CUSTOM;
            ptr = &t;
        }
    }

    [[nodiscard]] inline const trait_type& get_type() const OS_NOEXCEPT { return type; }

    [[nodiscard]] inline char get_char() const OS_NOEXCEPT { return value_char; }
    [[nodiscard]] inline bool get_bool() const OS_NOEXCEPT { return value_bool != 0; }
    [[nodiscard]] inline const char* get_str() const OS_NOEXCEPT { return value_str; }
    [[nodiscard]] inline int8_t get_int8() const OS_NOEXCEPT { return value_int8; }
    [[nodiscard]] inline uint8_t get_uint8() const OS_NOEXCEPT { return value_uint8; }
    [[nodiscard]] inline int16_t get_int16() const OS_NOEXCEPT { return value_int16; }
    [[nodiscard]] inline uint16_t get_uint16() const OS_NOEXCEPT { return value_uint16; }
    [[nodiscard]] inline int32_t get_int32() const OS_NOEXCEPT { return value_int32; }
    [[nodiscard]] inline uint32_t get_uint32() const OS_NOEXCEPT { return value_uint32; }
    [[nodiscard]] inline int64_t get_int64() const OS_NOEXCEPT { return value_int64; }
    [[nodiscard]] inline uint64_t get_uint64() const OS_NOEXCEPT { return value_uint64; }
    [[nodiscard]] inline float get_float() const OS_NOEXCEPT { return value_float; }
    [[nodiscard]] inline double get_double() const OS_NOEXCEPT { return value_double; }
    [[nodiscard]] inline const void*  get_ptr(size_t* size) const OS_NOEXCEPT { *size = ptr_size; return ptr; }

};

struct function_base
{
    using ptr = unique_ptr<function_base>;

    static constexpr const uint8_t MAX_ARGS = 3;

    enum type
    {
        NONE, METHOD, FUNCTION
    };

    function_base() = default;
    virtual ~function_base() = default;

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

    [[nodiscard]] inline type get_type() const OS_NOEXCEPT
    {
        return type;
    }

    virtual osal::exit execute(value* ret) const OS_NOEXCEPT = 0;

    virtual osal::exit execute(value* ret, void* a0) const OS_NOEXCEPT = 0;

    virtual osal::exit execute(value* ret, void* a0, void* a1) const OS_NOEXCEPT = 0;

    virtual osal::exit execute(value* ret, void* a0, void* a1, void* a2) const OS_NOEXCEPT = 0;

protected:
    enum type type = NONE;

    trait_type    args_type[MAX_ARGS]{trait_type::_VOID_, trait_type::_VOID_, trait_type::_VOID_};
    const uint8_t args_count   = 0;
    trait_type ret_type    = trait_type::_VOID_;

    inline explicit function_base(enum type type, const uint8_t args_count = 0, trait_type ret_type = trait_type::_VOID_) OS_NOEXCEPT
    : type(type), args_count(args_count), ret_type(ret_type)
    {
        memcpy(this->args_type, args_type, sizeof(this->args_type));
    }

public:


};

template<typename T, typename R, typename A0 = no_class, typename A1 = no_class, typename A2 = no_class>
class method final : public function_base
{

    T* target = nullptr;
    bool is_const = true;

    union method_prt
    {
        R (T::*method_no_arg)();

        R (T::*method_a0)(A0);

        R (T::*method_a0_a1)(A0, A1);

        R (T::*method_a0_a1_a2)(A0, A1, A2);

        R (T::*method_no_arg_const)() const;

		R (T::*method_a0_const)(A0) const;

		R (T::*method_a0_a1_const)(A0, A1) const;

		R (T::*method_a0_a1_a2_const)(A0, A1, A2) const;
    } method_prt;

public:

    using ptr = unique_ptr<method>;

    method(T* target, R (T::*method)()) OS_NOEXCEPT;

    method(T* target, R (T::*method)(A0)) OS_NOEXCEPT;

    method(T* target, R (T::*method)(A0, A1)) OS_NOEXCEPT;

    method(T* target, R (T::*method)(A0, A1, A2)) OS_NOEXCEPT;

    method(T* target, R (T::*method)() const) OS_NOEXCEPT;

    method(T* target, R (T::*method)(A0) const) OS_NOEXCEPT;

    method(T* target, R (T::*method)(A0, A1) const) OS_NOEXCEPT;

    method(T* target, R (T::*method)(A0, A1, A2) const) OS_NOEXCEPT;

    method(const method&) = default;

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

    osal::exit execute(value* ret) const OS_NOEXCEPT override;

    osal::exit execute(value* ret, void* a0) const OS_NOEXCEPT override;

    osal::exit execute(value* ret, void* a0, void* a1) const OS_NOEXCEPT override;

    osal::exit execute(value* ret, void* a0, void* a1, void* a2) const OS_NOEXCEPT override;
};

template<typename T, typename R, typename A0, typename A1, typename A2>
method<T, R, A0, A1, A2>::method(T* target, R (T::*method)()) OS_NOEXCEPT
: function_base(METHOD, 0, osal::get_type<R>::type), target(target)
{
    method_prt.method_no_arg = method;
}

template<typename T, typename R, typename A0, typename A1, typename A2>
method<T, R, A0, A1, A2>::method(T* target, R (T::*method)(A0)) OS_NOEXCEPT
: function_base(METHOD, 1, osal::get_type<R>::type), target(target)
{
    args_type[0] = osal::get_type<A0>::type;
    method_prt.method_a0 = method;
}

template<typename T, typename R, typename A0, typename A1, typename A2>
method<T, R, A0, A1, A2>::method(T* target, R (T::*method)(A0, A1)) OS_NOEXCEPT
        : function_base(METHOD, 2, osal::get_type<R>::type), target(target)
{
    args_type[0] = osal::get_type<A0>::type;
    args_type[1] = osal::get_type<A1>::type;
    method_prt.method_void_a0_a1 = method;
}

template<typename T, typename R, typename A0, typename A1, typename A2>
method<T, R, A0, A1, A2>::method(T* target, R (T::*method)(A0, A1, A2)) OS_NOEXCEPT
: function_base(METHOD, 3, osal::get_type<R>::type), target(target)
{
    args_type[0] = osal::get_type<A0>::type;
    args_type[1] = osal::get_type<A1>::type;
    args_type[2] = osal::get_type<A2>::type;
    method_prt.method_a0_01_a2 = method;
}

template<typename T, typename R, typename A0, typename A1, typename A2>
method<T, R, A0, A1, A2>::method(T* target, R (T::*method)() const) OS_NOEXCEPT
: function_base(METHOD, 0, osal::get_type<R>::type), target(target), is_const(true)
{
    method_prt.method_no_arg_const = method;
}


template<typename T, typename R, typename A0, typename A1, typename A2>
method<T, R, A0, A1, A2>::method(T* target, R (T::*method)(A0) const) OS_NOEXCEPT
: function_base(METHOD, 1, osal::get_type<R>::type), target(target), is_const(true)
{
    args_type[0] = osal::get_type<A0>::type;
    method_prt.method_a0_const = method;
}

template<typename T, typename R, typename A0, typename A1, typename A2>
method<T, R, A0, A1, A2>::method(T* target, R (T::*method)(A0, A1) const) OS_NOEXCEPT
        : function_base(METHOD, 2, osal::get_type<R>::type), target(target), is_const(true)
{
    args_type[0] = osal::get_type<A0>::type;
    args_type[1] = osal::get_type<A1>::type;
    method_prt.method_void_a0_a1_const = method;
}

template<typename T, typename R, typename A0, typename A1, typename A2>
method<T, R, A0, A1, A2>::method(T* target, R (T::*method)(A0, A1, A2) const) OS_NOEXCEPT
: function_base(METHOD, 3, osal::get_type<R>::type), target(target), is_const(true)
{
    args_type[0] = osal::get_type<A0>::type;
    args_type[1] = osal::get_type<A1>::type;
    args_type[2] = osal::get_type<A2>::type;
    method_prt.method_a0_01_a2_const = method;
}

template<typename T, typename R, typename A0, typename A1, typename A2>
osal::exit method<T, R, A0, A1, A2>::execute(value* ret) const OS_NOEXCEPT
{
	if constexpr (is_same<void, R> ::value)
	{
		if(!is_const)
			(target->*method_prt.method_no_arg)();
		else
			(target->*method_prt.method_no_arg_const)();
	}
	else
	{
		if(ret)
		{
			if(!is_const)
				*ret = (target->*method_prt.method_no_arg)();
			else
				*ret = (target->*method_prt.method_no_arg_const)();
		}
		else
			return exit::KO;
	}
	return exit::OK;
}

template<typename T, typename R, typename A0, typename A1, typename A2>
osal::exit method<T, R, A0, A1, A2>::execute(value* ret, void* a0) const OS_NOEXCEPT
{
	if constexpr (is_same<void, R> ::value)
	{
		if(!is_const)
			(target->*method_prt.method_a0)(*reinterpret_cast<A0*>(a0));
		else
			(target->*method_prt.method_a0_const)(*reinterpret_cast<A0*>(a0));
	}
	else
	{
		if(ret)
		{
			if(!is_const)
				*ret = (target->*method_prt.method_a0)(*reinterpret_cast<A0*>(a0));
			else
				*ret = (target->*method_prt.method_a0_const)(*reinterpret_cast<A0*>(a0));
		}
		else
			return exit::KO;
	}
	return exit::OK;
}

template<typename T, typename R, typename A0, typename A1, typename A2>
osal::exit method<T, R, A0, A1, A2>::execute(value* ret, void* a0, void* a1) const OS_NOEXCEPT
{
    if constexpr (is_same<void, R> ::value)
    {
    	if(!is_const)
    		(target->*method_prt.method_a0_a1)(*reinterpret_cast<A0*>(a0), *reinterpret_cast<A1*>(a1));
    	else
    		(target->*method_prt.method_a0_a1_const)(*reinterpret_cast<A0*>(a0), *reinterpret_cast<A1*>(a1));
    }
    else
    {
        if(ret)
        {
        	if(!is_const)
        		*ret = (target->*method_prt.method_a0_a1)(*reinterpret_cast<A0*>(a0), *reinterpret_cast<A1*>(a1));
        	else
        		*ret = (target->*method_prt.method_a0_a1_const)(*reinterpret_cast<A0*>(a0), *reinterpret_cast<A1*>(a1));
        }
        else
            return exit::KO;
    }
    return exit::OK;

}

template<typename T, typename R, typename A0, typename A1, typename A2>
osal::exit method<T, R, A0, A1, A2>::execute(value* ret, void* a0, void* a1, void* a2) const OS_NOEXCEPT
{
    if constexpr (is_same<void, R> ::value)
    {
    	if(!is_const)
    		(target->*method_prt.method_a0_a1_a2)(*reinterpret_cast<A0*>(a0), *reinterpret_cast<A1*>(a1), *reinterpret_cast<A2*>(a2));
    	else
    		(target->*method_prt.method_a0_a1_a2_const)(*reinterpret_cast<A0*>(a0), *reinterpret_cast<A1*>(a1), *reinterpret_cast<A2*>(a2));
    }
    else
    {
        if(ret)
        {
        	if(!is_const)
        		*ret = (target->*method_prt.method_a0_a1_a2)(*reinterpret_cast<A0*>(a0), *reinterpret_cast<A1*>(a1), *reinterpret_cast<A2*>(a2));
        	else
        		*ret = (target->*method_prt.method_a0_a1_a2_const)(*reinterpret_cast<A0*>(a0), *reinterpret_cast<A1*>(a1), *reinterpret_cast<A2*>(a2));
        }
        else
            return exit::KO;
    }
    return exit::OK;
}

template<typename R, typename A0 = no_class, typename A1 = no_class, typename A2 = no_class>
class function final : public function_base
{
    union function_prt
    {
        R (* function_no_arg)();

        R (* function_a0)(A0);

        R (* function_a0_a1)(A0, A1);

        R (* function_a0_a1_a2)(A0, A1, A2);
    } function_prt{nullptr};

public:
    using ptr = unique_ptr<function>;

    explicit function(R (* function)()) OS_NOEXCEPT;

    explicit function(R (* function)(A0)) OS_NOEXCEPT;

    explicit function(R (* function)(A0, A1)) OS_NOEXCEPT;

    explicit function(R (* function)(A0, A1, A2)) OS_NOEXCEPT;

    function(const function&) = delete;

    function& operator=(const function&) = delete;

    function(function&&) = delete;

    function& operator=(function&&) = delete;

    [[nodiscard]] inline const union function_prt& get_function() const OS_NOEXCEPT
    {
        return function_prt;
    }

    osal::exit execute(value* ret) const OS_NOEXCEPT override;

    osal::exit execute(value* ret, void* a0) const OS_NOEXCEPT override;

    osal::exit execute(value* ret, void* a0, void* a1) const OS_NOEXCEPT override;

    osal::exit execute(value* ret, void* a0, void* a1, void* a2) const OS_NOEXCEPT override;
};

template<typename R, typename A0, typename A1, typename A2>
function<R, A0, A1, A2>::function(R (* function)()) OS_NOEXCEPT
: function_base(FUNCTION, 0, osal::get_type<R>::type)
{
    function_prt.function_no_arg = function;
}

template<typename R, typename A0, typename A1, typename A2>
function<R, A0, A1, A2>::function(R (* function)(A0)) OS_NOEXCEPT
: function_base(FUNCTION, 1, osal::get_type<R>::type)
{
    args_type[0] = osal::get_type<A0>::type;
    function_prt.function_a0 = function;
}


template<typename R, typename A0, typename A1, typename A2>
function<R, A0, A1, A2>::function(R (* function)(A0, A1)) OS_NOEXCEPT
: function_base(FUNCTION, 2, osal::get_type<R>::type)
{
    args_type[0] = osal::get_type<A0>::type;
    args_type[1] = osal::get_type<A1>::type;
    function_prt.function_a0_a1 = function;
}

template<typename R, typename A0, typename A1, typename A2>
function<R, A0, A1, A2>::function(R (* function)(A0, A1, A2)) OS_NOEXCEPT
: function_base(FUNCTION, 3, osal::get_type<R>::type)
{
    args_type[0] = osal::get_type<A0>::type;
    args_type[1] = osal::get_type<A1>::type;
    args_type[2] = osal::get_type<A2>::type;
    function_prt.function_a0_a1_a2 = function;
}

template<typename R, typename A0, typename A1, typename A2>
osal::exit function<R, A0, A1, A2>::execute(value* ret) const OS_NOEXCEPT
{
    if constexpr (is_same<void, R> ::value)
    {
        function_prt.function_no_arg();
    }
    else
    {
        if(ret)
            *ret = function_prt.function_no_arg();
        else
            return exit::KO;
    }

    return exit::OK;
}

template<typename R, typename A0, typename A1, typename A2>
osal::exit function<R, A0, A1, A2>::execute(value* ret, void* a0) const OS_NOEXCEPT
{
    if constexpr (is_same<void, R> ::value)
    {
        function_prt.function_a0(*reinterpret_cast<A0*>(a0));
    }
    else
    {
        if(ret)
            *ret = function_prt.function_a0(*reinterpret_cast<A0*>(a0));
        else
            return exit::KO;
    }
    return exit::OK;
}

template<typename R, typename A0, typename A1, typename A2>
osal::exit function<R, A0, A1, A2>::execute(value* ret, void* a0, void* a1) const OS_NOEXCEPT
{

    if constexpr (is_same<void, R> ::value)
    {
        function_prt.function_a0_a1(*reinterpret_cast<A0*>(a0), *reinterpret_cast<A1*>(a1));
    }
    else
    {
        if(ret)
            *ret = function_prt.function_a0_a1(*reinterpret_cast<A0*>(a0), *reinterpret_cast<A1*>(a1));
        else
            return exit::KO;
    }
    return exit::OK;
}

template<typename R, typename A0, typename A1, typename A2>
osal::exit function<R, A0, A1, A2>::execute(value* ret, void* a0, void* a1, void* a2) const OS_NOEXCEPT
{
    if constexpr (is_same<void, R> ::value)
    {
        function_prt.function_a0_a1_a2(*reinterpret_cast<A0*>(a0), *reinterpret_cast<A1*>(a1), *reinterpret_cast<A2*>(a2));
    }
    else
    {
        if(ret)
            *ret = function_prt.function_a0_a1_a2(*reinterpret_cast<A0*>(a0), *reinterpret_cast<A1*>(a1), *reinterpret_cast<A2*>(a2));
        else
            return exit::KO;
    }
    return exit::OK;
}

}
}
