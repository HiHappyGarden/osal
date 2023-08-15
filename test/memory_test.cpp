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

struct base
{
    int a;
    int b;
    int c;
};

TEST(memory_test, base)
{
    os::unique_ptr test = new base{1,2,3};

    ASSERT_EQ(test->a, 1);
    ASSERT_EQ(test->b, 2);
    ASSERT_EQ(test->c, 3);

}

TEST(memory_test, deleter)
{
    os::unique_ptr<base, void(*)(base*)> test(new base{1,2,3}, [](auto ptr) { delete ptr; });

    ASSERT_EQ(test->a, 1);
    ASSERT_EQ(test->b, 2);
    ASSERT_EQ(test->c, 3);

    base& b = *test;
    ASSERT_EQ(test->a, b.a);
    ASSERT_EQ(test->b, b.b);
    ASSERT_EQ(test->c, b.c);

    auto release = test.release();

    ASSERT_EQ(release->a, b.a);
    ASSERT_EQ(release->b, b.b);
    ASSERT_EQ(release->c, b.c);    
}

TEST(memory_test, array)
{
    os::unique_ptr test( new int[10], os::array_deleter_int);
    for(int i = 0; i < 10; i++)
    {
        test[i] = i;
        ASSERT_EQ(test[i], i);
    }
}

struct virt
{
    ~virt() = default;

    virtual int get_i() = 0;
};

struct derived : public virt
{

    int i;
    derived(int i) : i(i) {}
    ~derived() = default;


    int get_i() override
    {
        return i;
    }
};


TEST(memory_test, switch)
{
    os::unique_ptr<virt> test1 = new derived(1);

    test1 = new derived(2);

    ASSERT_EQ(test1->get_i(), 2);
}

