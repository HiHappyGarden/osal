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
#include <ctime>
namespace osal
{

constexpr const uint8_t TIMER_PRIO = 30;
constexpr const uint32_t USECS_PER_SEC = (1 * 1000 * 1000);
constexpr const uint32_t NSECS_PER_SEC = (1 * 1000 * 1000 * 1000);

void usleep (uint64_t us) OS_NOEXCEPT
{

}

uint64_t get_current_time_us (void) noexcept
{
    struct timespec ts;

    clock_gettime (CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000 * 1000 + ts.tv_nsec / 1000;
}

tick tick_current (void) OS_NOEXCEPT
{
    struct timespec ts;
    tick       tick;

    clock_gettime (CLOCK_MONOTONIC, &ts);
    tick = ts.tv_sec;
    tick *= NSECS_PER_SEC;
    tick += ts.tv_nsec;
    return tick;
}

tick tick_from_us (uint64_t us) OS_NOEXCEPT
{

}

void tick_sleep (tick tick) OS_NOEXCEPT
{

}
