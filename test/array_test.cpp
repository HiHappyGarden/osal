#include <gtest/gtest.h>

#include"osal/osal.hpp"

#include <stdio.h>
#include <array>

TEST(array_test, size)
{
//    auto i = {1,2,3,4,5};
//    os::array<int, 4> str = i;

//    ASSERT_EQ(str.length(), 4);
//    ASSERT_EQ(str.size(), 4);

    //Testone t = 20;

   //f.fill();

    os::array_init data(1,2,3,4,5,6);
    os::array<int, 10> data2;

    const int* i = data.get_data();


    std::cout << *i << std::endl;
    i++;

    std::cout << *i << std::endl;
    i++;

    std::cout << *i << std::endl;
    i++;
}

TEST(array_test, char_array)
{

    os::string<128> str;
    ASSERT_EQ(str.size(), 127);

    str += "hello world";


    char s[] = " by me";
    str += s;

    ASSERT_EQ(strcmp(str.c_str(), "hello world by me"), 0);

    {
        auto check = os::string("test") == "test";
        ASSERT_TRUE(check);
    }

    {
        auto check = os::string("test") != "test";
        ASSERT_FALSE(check);
    }
}


TEST(array_test, char_ptr)
{

    os::string<128> str;
    ASSERT_EQ(str.size(), 127);


    str += "hello world";

    char *p = " by me";
    str += p;


    char* s = "hello world by me";
    ASSERT_EQ(strcmp(str.c_str(), s), 0);

    char* t = "test";
    {
        auto check = os::string("test") == t;
        ASSERT_TRUE(check);
    }

    {
        auto check = os::string("test") != t;
        ASSERT_FALSE(check);
    }
}


TEST(array_test, unarray)
{
    os::string s("test");

    char* c =  s[1];
    ASSERT_EQ(*c, 'e');
    c = s[4];
    ASSERT_TRUE(c == nullptr);
}

TEST(array_test, assignment)
{
    os::string s = "test";

    ASSERT_EQ(s.size(), 4);
    ASSERT_EQ(s.length(), 4);
    ASSERT_TRUE(s == "test");

    s = "ciao";
    ASSERT_EQ(s.size(), 4);
    ASSERT_EQ(s.length(), 4);
    ASSERT_TRUE(s == "ciao");

}
