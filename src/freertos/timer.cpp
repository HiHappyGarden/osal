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
#include "osal/timer.hpp"
#include "osal/thread.hpp"



namespace osal
{
inline namespace v1
{

namespace
{
constexpr const uint8_t TIMER_PRIO = 30;
}

void* timer_thread (void * arg)
{
    return nullptr;
}



timer::timer(uint64_t us, handler fn, bool oneshot) OS_NOEXCEPT
    : us(us)
    , fn(fn)
    , oneshot(oneshot)
{

}

timer::~timer() OS_NOEXCEPT
{

}

exit timer::create(void *arg, error** error)
{
    return exit::OK;
}

void timer::set(uint64_t us) OS_NOEXCEPT
{

}

void timer::start() const OS_NOEXCEPT
{

}

void timer::start_from_isr() const OS_NOEXCEPT
{
    start();
}

void timer::stop() const OS_NOEXCEPT
{

}

void timer::stop_from_isr() const OS_NOEXCEPT
{
    stop();
}



}
}
