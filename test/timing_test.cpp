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
constexpr inline const char APP_TAG[] = "TIMING TEST";
}

TEST(timing_test, check_sleep)
{
    osal::log_debug(APP_TAG, "start");
    os::us_sleep(1'000'000);
    osal::log_debug(APP_TAG, "end");
}

TEST(timing_test, base)
{

    os::thread thread_start_main_loop("test", 4, OASL_TASK_HEAP, [](void*) -> void*
    {

        check = true;

        os::us_sleep(1'000'000);


        return nullptr;
    });

    os::thread thread_stop_main_loop("test", 4, OASL_TASK_HEAP, [](void*) -> void*
    {

        os::tick_sleep(2);

        os::stop_main_loop();

        return nullptr;
    });


    ASSERT_EQ(thread_start_main_loop.create(), osal::exit::OK);
    ASSERT_EQ(thread_stop_main_loop.create(), osal::exit::OK);

    os::start_main_loop();


    ASSERT_TRUE(check);
}

