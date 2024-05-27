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

TEST(string_test, size)
{
    os::string str("help");

    ASSERT_EQ(str.length(), 4);
    ASSERT_EQ(str.size(), 4);

    for(auto&& it : str)
    {
        printf("it:%d", it);
    }


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

    const char *p = " by me";
    str += p;


    const char* s = "hello world by me";
    ASSERT_EQ(strcmp(str.c_str(), s), 0);

    const char* t = "test";
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

    char c =  s[1];
    ASSERT_EQ(c, 'e');
    c = s[4];
    ASSERT_TRUE(c == '\0');
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


TEST(string_test, find)
{
    os::string s = "test sono antonio";

    ASSERT_TRUE(strcmp(s.find("sono"), "sono antonio") == 0);

}

TEST(string_test, start)
{
    os::string s = "test sono antonio";

    ASSERT_TRUE(s.start_with("test"));

}

TEST(string_test, rtrim)
{
    os::string s = "sono antonio \r\n";

    ASSERT_TRUE(strcmp(s.rtrim().c_str(), "sono antonio") == 0);
}

TEST(string_test, strstr)
{
    os::string buffer = " $ciao sono antonio \r\n";

    auto start = buffer.find("$");
    auto end = buffer.find("\r\n");

    size_t idx = start - buffer.c_str();
    size_t offset = end - start;

    auto&& rc = buffer.strstr(start - buffer.c_str(), end - start);

    ASSERT_TRUE(strcmp(rc.c_str(), "$ciao sono antonio ") == 0);


    rc = buffer.strstr(2, 4);

    ASSERT_TRUE(strcmp(rc.c_str(), "ciao") == 0);

}

TEST(string_test, replace)
{
os::string s = "sono antonio \r\n";

auto rc = s.replace("sono", "ciao");

ASSERT_TRUE(strcmp(rc, "ciao antonio") == 0);

delete rc;

}