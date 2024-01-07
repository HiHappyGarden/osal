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
using namespace osal;

#include <stdio.h>



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
        return i > 0;
    }
};

void func1()
{
    std::cout << "func1:" << std::endl;
}

char func2(int a1, int a2)
{
    std::cout << "func2:" << (a1 + a2) << std::endl;
    return (a1 + a2);
}

class obj
{
    void operator()()
    {
        std::cout << "operator():" << (1) << std::endl;
    }
};



TEST(generics_test, dynamic_function)
{
    test1 t1;
    test2 t2;

    method m1(&t1, &test1::test_1, trait_type::_VOID_);
    int32_t i =0;
    method m2(&t2, &test2::test_2, trait_type::BOOL, trait_type::INT32);
    function f1 (func1, trait_type::_VOID_);
    function f2 (func2, trait_type::CHAR, trait_type::INT32, trait_type::INT32);

    void (*f)() = [] {};
    function l2 (f, trait_type::_VOID_);

    function_base* a1[4];
    a1[0] = &m1;
    a1[1] = &m2;
    a1[2] = &f1;
    a1[3] = &f2;

    const function_base& fb1 = m1;
    function_base&& fb2 = method(&t2, &test2::test_2, trait_type::BOOL, trait_type::INT32);;


    (m1.get_target()->*m1.get_method().method_no_arg)();
    bool r = (m2.get_target()->*m2.get_method().method_a0)(30);

    ASSERT_TRUE(r);

    std::cout << "ret:" << std::to_string(r) << std::endl;
}


TEST(generics_test, function_base_call)
{
    test1 t1;
    test2 t2;

    function_base::ptr m1 = new method(&t1, &test1::test_1, trait_type::_VOID_);
    function_base::ptr m2 = new method(&t2, &test2::test_2, trait_type::BOOL, trait_type::INT32);

    function_base::ptr f1 = new function (func1, trait_type::_VOID_);
    function_base::ptr f2 = new function (func2, trait_type::CHAR, trait_type::INT32, trait_type::INT32);

    {
        m1->execute(nullptr, nullptr, nullptr, nullptr);

        value ret;
        int32_t p0;
        m2->execute(&ret, &p0, nullptr, nullptr);

        ASSERT_TRUE(ret.get_bool());
    }

    {
        value ret;
        int32_t p0 = 10;
        int32_t p1 = 20;
        f2->execute(&ret, &p0, &p1, nullptr);

        ASSERT_TRUE(ret.get_int8());


    }

}