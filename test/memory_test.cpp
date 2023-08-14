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
    os::unique_ptr<base> test(new base{1,2,3});

    ASSERT_EQ(test->a, 1);
    ASSERT_EQ(test->b, 2);
    ASSERT_EQ(test->c, 3);

}

class test_deleter : public os::default_delete<base>
{
    test_deleter() = delete;
    void operator()(base* ptr) const { delete ptr; }
};

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
