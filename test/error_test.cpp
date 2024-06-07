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

constexpr const char APP_TAG[]  = "ERROR TEST";

TEST(error_test, base)
{

    auto err = OS_ERROR_BUILD("test", 20);
    OS_ERROR_PTR_SET_POSITION(err);

    ASSERT_TRUE(strcmp(err->get_msg(), "test") == 0);
    ASSERT_TRUE(strcmp(err->get_file(), "error_test.cpp") == 0);
    ASSERT_TRUE(strcmp(err->get_func(), "TestBody") == 0);
    ASSERT_TRUE(err->get_line() == 29);

    delete err;
}


TEST(error_test, old_error)
{

    auto old = OS_ERROR_BUILD("old", 20);
    
    auto err = new osal::error{old, "test",30, osal::get_file_name(__FILE__), __FUNCTION__, __LINE__};

    os::printf_stack_error(APP_TAG, err);


    delete err;
}

