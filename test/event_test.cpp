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


#define BIT1 0x01
#define BIT2 0x02

static void* thread_1 (void * arg);

static os::thread thread1{"test_1", 4, 1024, thread_1};
static os::thread thread2{"test_2", 4, 1024, thread_1};
os::event event;
static bool run = true;

void* thread_1 (void * arg)
{
    while(run)
    {
        uint32_t v = BIT1;
        event.wait(BIT1|BIT2, v, os::WAIT_FOREVER);
        if(v & BIT1)
        {

            os::tick_sleep(os::sec_to_us(1));

            event.clear(BIT1);
            event.set(BIT2);
        }
    }

    return nullptr;
}

static void thread_2 (void * arg)
{
    while(run)
    {
        uint32_t v = BIT2;
        event.wait(BIT1|BIT2, v, os::WAIT_FOREVER);
        if(v & BIT2)
        {

            run = false;

            os::tick_sleep(os::sec_to_us(1));

            event.clear(BIT2);
            event.set(BIT1);
        }
    }

    EXPECT_EQ(run, false);
}

TEST(event_test, two_thread_synch_event)
{


    char args[] = "args";

    thread1.create(args);

    os::tick_sleep(os::sec_to_us(1));

    thread1.create();

    os::tick_sleep(os::ms_to_us(500));

    event.set(BIT1);

    os::tick_sleep(os::ms_to_us(500));

}


TEST(event_test, test_set_get)
{

    os::event event1;

    event1.set(BIT1);

    ASSERT_EQ(event1.get(), BIT1);
}
