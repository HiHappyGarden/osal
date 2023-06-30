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


TEST(error_test, base)
{

    os::error* err = OSAL_BUILD_ERROR("test", 20);
    err->set_position();

    ASSERT_TRUE(strcmp(err->get_msg(), "test") == 0);
    ASSERT_TRUE(strcmp(err->get_file(), "error_test.cpp") == 0);
    ASSERT_TRUE(strcmp(err->get_func(), "TestBody") == 0);
    ASSERT_TRUE(err->get_line() == 10);

    delete err;
}


TEST(error_test, old_error)
{

    os::error* old = OSAL_BUILD_ERROR("old", 20);


    os::error err(*old, "test",30, osal::get_file_name(__FILE__), __FUNCTION__, __LINE__);

    os::printf_stack_error(err);


    delete old;
}

