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
    bool done = false;
    int32_t check = 0;

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
    timespec ts{0};
    timespec remain{0};

    ts.tv_sec  = tick / NSECS_PER_SEC;
    ts.tv_nsec = tick % NSECS_PER_SEC;
    while (clock_nanosleep (CLOCK_MONOTONIC, 0, &ts, &remain) != 0)
    {
        ts = remain;
    }
}

void start_main_loop() OS_NOEXCEPT
{
    signal(SIGINT, ctrl_c_handler);
    while(!done) {

        //blocking check
        while (!done && !check)
        {
            sleep(1);
        }
        check = 0;
    }
}

void stop_main_loop() OS_NOEXCEPT
{
    check = 0;
}

}
}
