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

