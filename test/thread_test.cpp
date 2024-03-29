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
#include"common_test.hpp"

namespace
{

bool check = false;


void *thread_test(void *)
{
    check = true;
    return nullptr;
}

}


TEST(thread_test, base)
{
    os::thread thread("test", 4, OASL_TASK_HEAP, thread_test);

    ASSERT_EQ(thread.create(), osal::exit::OK);

    os::us_sleep(1'000'000);

    ASSERT_TRUE(check);
}

