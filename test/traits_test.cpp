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
#include <gtest/gtest.h>

#include"osal/osal.hpp"

using namespace os;

class test_type {};

TEST(traits_test, is_type)
{

    ASSERT_EQ(is_type<uint8_t>::type, trait_type::UINT8);
    ASSERT_EQ(is_type<char*>::type, trait_type::STR);
    ASSERT_EQ(is_type<string<10>>::type, trait_type::STRING);
    ASSERT_EQ(is_type<string<10>>::type, trait_type::STRING);

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

TEST(traits_test, dynamic_function)
{
    test1 t1;
    test2 t2;

    method m1(&t1, &test1::test_1);
    method m2(&t2, &test2::test_2);
    function f1 (func1);
    function f2 (func2);

    func_reference* a1[4];
    a1[0] = &m1;
    a1[1] = &m2;
    a1[2] = &f1;
    a1[3] = &f2;



}