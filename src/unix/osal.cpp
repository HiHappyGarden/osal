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


#ifdef __MACH__
#include <mach/clock_types.h>
#include <assert.h>
#include <errno.h>
#endif

namespace osal
{
inline namespace v1
{


constexpr const uint8_t TIMER_PRIO = 30;
constexpr const uint32_t USECS_PER_SEC = 1'000'000;
constexpr const uint32_t NSECS_PER_SEC = 1'000'000'000;


#ifdef __MACH__

namespace
{

constexpr const int TIMER_ABSTIME = 12345;

extern "C" void __timespec_diff(const struct timespec* lhs,
                     const struct timespec* rhs,
                     struct timespec* out) {
    assert(lhs->tv_sec <= rhs->tv_sec);

    out->tv_sec = rhs->tv_sec - lhs->tv_sec;
    out->tv_nsec = rhs->tv_nsec - lhs->tv_nsec;

    if (out->tv_sec < 0) {
        out->tv_sec = 0;
        out->tv_nsec = 0;
        return;
    }

    if (out->tv_nsec < 0) {
        if (out->tv_sec == 0) {
            out->tv_sec = 0;
            out->tv_nsec = 0;
            return;
        }

        out->tv_sec = out->tv_sec - 1;
        out->tv_nsec = out->tv_nsec + NSEC_PER_SEC;
    }
}


extern "C" int clock_nanosleep(clockid_t clock_id,
                    int flags,
                    const struct timespec *req,
                    struct timespec *rem) {

    assert(clock_id == CLOCK_REALTIME || clock_id == CLOCK_MONOTONIC);
    assert(0 <= req->tv_nsec && req->tv_nsec <= NSEC_PER_SEC);
    assert(flags == 0 || flags == TIMER_ABSTIME);
    assert(flags != TIMER_ABSTIME || clock_id == CLOCK_MONOTONIC);

    if (flags == TIMER_ABSTIME) {
        struct timespec now;
        struct timespec diff;

        if (clock_gettime(clock_id, &now) != 0) {
            return errno;
        }

        __timespec_diff(&now, req, &diff);

        return nanosleep(&diff, rem);
    }

    if (nanosleep(req, rem) != 0) {
        return errno;
    }

    return 0;
}

}
#endif

void us_sleep (uint64_t usec) OS_NOEXCEPT
{
    struct timespec ts;
    struct timespec remain;

    ts.tv_sec  = usec / USECS_PER_SEC;
    ts.tv_nsec = (usec % USECS_PER_SEC) * 1'000;
    while (clock_nanosleep (CLOCK_MONOTONIC, 0, &ts, &remain) != 0)
    {
        ts = remain;
    }
}

uint64_t get_current_time_us (void) noexcept
{
    struct timespec ts;

    clock_gettime (CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1'000'000 + ts.tv_nsec / 1'000;
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
    return static_cast<tick>(us) * 1'000;
}

void tick_sleep (tick tick) OS_NOEXCEPT
{
    struct timespec ts;
    struct timespec remain;

    ts.tv_sec  = tick / NSECS_PER_SEC;
    ts.tv_nsec = tick % NSECS_PER_SEC;
    while (clock_nanosleep (CLOCK_MONOTONIC, 0, &ts, &remain) != 0)
    {
        ts = remain;
    }
}
}
}
