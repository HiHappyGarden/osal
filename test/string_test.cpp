#include <gtest/gtest.h>

#include"osal/osal.hpp"

#include <stdio.h>

TEST(string_test, size)
{
    os::string str("help");

    ASSERT_EQ(str.length(), 4);
    ASSERT_EQ(str.size(), 4);


}

TEST(string_test, char_array)
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


TEST(string_test, char_ptr)
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


TEST(string_test, unarray)
{
    os::string s("test");

    char* c =  s[1];
    ASSERT_EQ(*c, 'e');
    c = s[4];
    ASSERT_TRUE(c == nullptr);
}

TEST(string_test, assignment)
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
