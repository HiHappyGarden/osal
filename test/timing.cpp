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

namespace
{

bool check = false;

}


TEST(thiming_test, base)
{
    os::thread thread_start_main_loop("test", 4, 1024, [](void*) -> void*
    {

        os::start_main_loop();

        check = true;

        return nullptr;
    });

    os::thread thread_stop_main_loop("test", 4, 1024, [](void*) -> void*
    {

        os::tick_sleep(2);

        os::stop_main_loop();

        return nullptr;
    });


    ASSERT_TRUE(thread_start_main_loop.create());
    ASSERT_TRUE(thread_stop_main_loop.create());

    os::us_sleep(10'000'000);

    ASSERT_TRUE(check);
}

