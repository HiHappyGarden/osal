#include <gtest/gtest.h>

#include"osal/osal.hpp"

#include <stdio.h>

TEST(array_test, size)
{
    os::array<int, 6> data2;
    ASSERT_EQ(data2.get_size(), 6);
    ASSERT_EQ(data2.get_length(), 0);

    os::array_init data1(1,2,3,4,5,6);
    ASSERT_EQ(data1.size(), 6);
    ASSERT_EQ(data1.length(), 6);
}

TEST(array_test, char_array)
{

    os::array<int, 6> data2;
    data2 << 10;
    ASSERT_EQ(data2.get_size(), 6);
    ASSERT_EQ(data2.get_length(), 1);
    ASSERT_EQ(data2[0], 10);

    os::array_init data1(1,2,3,4,5,6);
    data1 << 10;
    ASSERT_EQ(data1.size(), 6);
    ASSERT_EQ(data1.length(), 6);
    ASSERT_EQ(data1[0], 1);
    //ASSERT_TRUE(data1[6] == nullptr);


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

