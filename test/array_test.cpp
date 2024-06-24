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

#include <stdio.h>

TEST(array_test, size)
{
    os::array<int, 6> data2;
    ASSERT_EQ(data2.get_size(), 6);
    ASSERT_EQ(data2.get_length(), 0);

    os::range data1(1,2,3,4,5,6);
    ASSERT_EQ(data1.size(), 6);
    ASSERT_EQ(data1.length(), 6);

    for(auto&& it : data2)
    {
        printf("it:%d", it);
    }
}

TEST(array_test, char_array)
{

    os::array<int, 6> data2{99};
    data2 << 10;
    ASSERT_EQ(data2.get_size(), 6);
    ASSERT_EQ(data2.get_length(), 1);
    ASSERT_EQ(data2[0], 10);
    ASSERT_TRUE(data2[10] == 99);

    os::range data1(1,2,3,4,5,6);
    data1.set_default_value(99);
    data1 << 10;
    ASSERT_EQ(data1.size(), 6);
    ASSERT_EQ(data1.length(), 6);
    ASSERT_EQ(data1[0], 1);

    ASSERT_TRUE(data1[10] == 99);


}


TEST(array_test, char_ptr)
{

    os::array<int, 6> data;
    data << 1;
    data << 2;
    data << 3;
    data << 4;
    data << 5;
    data << 6;
    data << 7;
    ASSERT_EQ(data.get_size(), 6);
    ASSERT_EQ(data.get_length(), 6);
    ASSERT_EQ(data[0], 1);

}

