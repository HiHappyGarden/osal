/***************************************************************************
 *
 * OSAL
 * Copyright (C) 2023 / 2024  Antonio Salsi <passy.linux@zresa.it>
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
#include <gtest/gtest.h>

#include"osal/osal.hpp"

using namespace os;

class test_type {};

TEST(traits_test, get_type)
{

    ASSERT_EQ(get_type<uint8_t>::type, trait_type::UINT8);
    ASSERT_EQ(get_type<char*>::type, trait_type::STR);
    ASSERT_EQ(get_type<string<10>>::type, trait_type::STRING);
    ASSERT_EQ(get_type<string<10>>::type, trait_type::STRING);

}

class smart_call_test
{
public:
    void test_func(int i)
    {
        printf("test_func %d", i);
        fflush(stdout);
    }

};

TEST(traits_test, smart_call)
{

    smart_call_test t;

    smart_call<smart_call_test, void, int>::invoke(&t, &smart_call_test::test_func, 43);

}

struct test1
{
    void test_1()
    {
        std::cout << "test1" << std::endl;
    }
};

struct test2
{
    bool test_2(int i)
    {
        std::cout << "test2:" << i << std::endl;
        return true;
    }
};

void func1()
{
    std::cout << "func1:" << std::endl;
}

char func2(int a1, int a2)
{
    std::cout << "func2:" << (a1 + a2) << std::endl;
    return 30;
}

class obj
{
    void operator()()
    {
        std::cout << "operator():" << (1) << std::endl;
    }
};



TEST(traits_test, dynamic_function)
{
    test1 t1;
    test2 t2;

    method m1(&t1, &test1::test_1, trait_type::VOID);
    int32_t i =0;
    method m2(&t2, &test2::test_2, trait_type::BOOL, trait_type::INT32);
    function f1 (func1, trait_type::VOID);
    function f2 (func2, trait_type::CHAR, trait_type::INT32, trait_type::INT32);

    void (*f)() = [] {};
    function l2 (f, trait_type::VOID);

    function_base* a1[4];
    a1[0] = &m1;
    a1[1] = &m2;
    a1[2] = &f1;
    a1[3] = &f2;

    const function_base& fb1 = m1;
    function_base&& fb2 = method(&t2, &test2::test_2, trait_type::BOOL, trait_type::INT32);;

    
}