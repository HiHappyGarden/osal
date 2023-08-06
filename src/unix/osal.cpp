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
#include "osal/osal.hpp"

#include <time.h>
#include <signal.h>

namespace osal
{
inline namespace v1
{

namespace
{

uint8_t main_loop_sleep = 1;

bool main_loop_started = false;
bool done = false;
bool check = false;

void ctrl_c_handler(int n)
{
    if (n == SIGINT)
    {
        done = true;
    }
}

}


template<typename T>
constexpr T max(T a, T b)
{
    return a > b ? a : b;
}

template<typename T>
constexpr T min(T a, T b)
{
    return a < b ? a : b;
}

void us_sleep (uint64_t usec) OS_NOEXCEPT
{
    timespec ts{0};
    timespec remain{0};

    ts.tv_sec  = usec / USECS_PER_SEC;
    ts.tv_nsec = (usec % USECS_PER_SEC) * 1'000;
    while (clock_nanosleep (CLOCK_MONOTONIC, 0, &ts, &remain) != 0)
    {
        ts = remain;
    }
}

uint64_t get_current_time_us (void) OS_NOEXCEPT
{
    timespec ts{0};

    clock_gettime (CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1'000'000 + ts.tv_nsec / 1'000;
}

tick tick_current (void) OS_NOEXCEPT
{
    timespec ts{0};
    tick       tick;

    clock_gettime (CLOCK_MONOTONIC, &ts);
    tick = ts.tv_sec;
    tick *= NSECS_PER_SEC;
    tick += ts.tv_nsec;
    return tick;
}

tick tick_from_us (uint64_t us) OS_NOEXCEPT
{
    return static_cast<tick>(us) * 1'000;
}

void tick_sleep (tick tick) OS_NOEXCEPT
{
    if(!main_loop_started)
    {
        timespec ts{0};
        timespec remain{0};

        ts.tv_sec  = tick / NSECS_PER_SEC;
        ts.tv_nsec = tick % NSECS_PER_SEC;
        while (clock_nanosleep (CLOCK_MONOTONIC, 0, &ts, &remain) != 0)
        {
            ts = remain;
        }
    }
    else
    {
        osal::tick counter = 0;
        while(!done && (counter < tick))
        {

            //blocking check
            while (!done && !check && (counter < tick))
            {
                sleep(main_loop_sleep);
                counter += main_loop_sleep;
            }
            check = false;
        }
        main_loop_started = false;
        done = false;
    }

}

void set_sleep_main_loop(uint16_t sleep_timing) OS_NOEXCEPT
{
    main_loop_sleep = sleep_timing;
}

uint16_t get_sleep_main_loop() OS_NOEXCEPT
{
    return main_loop_sleep;
}

void set_check_main_loop(bool check) OS_NOEXCEPT
{
    osal::check = check;
}

void start_main_loop() OS_NOEXCEPT
{
    main_loop_started = true;
    signal(SIGINT, ctrl_c_handler);
    while(!done)
    {

        //blocking check
        while (!done && !check)
        {
            sleep(main_loop_sleep);
        }
        check = false;
    }
    main_loop_started = false;
    done = false;
}

void stop_main_loop() OS_NOEXCEPT
{
    done = true;
}



}
}
