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

#include <string.h>

static char args[] = "args 1";

os::timer timer{1000
                , [](auto timer, auto args) ->void*
                {
                    EXPECT_EQ(strcmp(static_cast<char *>(args), "args 1"), 0);
                    return nullptr;
                }
};


TEST(timer_test, single_timer)
{

    if(timer.create(args) == osal::exit::KO)
    {
        OS_LOG_ERROR("TIMER", "ko");
        FAIL();
    }

    timer.start();

}
