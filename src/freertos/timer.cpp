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
#include "osal/timer.hpp"
#include "osal/thread.hpp"



namespace osal
{
inline namespace v1
{

void timer_data::args_wrapper::wrap_func( TimerHandle_t timer )
{
    if(timer == nullptr)
    {
        return;
    }

    auto wrapper = static_cast<timer_data::args_wrapper*>(pvTimerGetTimerID (timer));
    wrapper->fn(wrapper->timer, wrapper->arg);
}

timer::timer(uint64_t us, handler fn, bool one_shot) OS_NOEXCEPT
    : us(us)
    , fn(fn)
    , one_shot(one_shot)
{
    t.args_wrp.fn = fn;
    t.args_wrp.timer = this;
}

timer::~timer() OS_NOEXCEPT
{
    if(t.handler)
    {
        xTimerDelete (t.handler, portMAX_DELAY);
        t.handler = nullptr;
    }
}

exit timer::create(void *arg, error** error) OS_NOEXCEPT
{
    t.args_wrp.arg = arg;
    t.handler = xTimerCreate (
            "os_timer",
            (us / portTICK_PERIOD_MS) / 1'000,
            one_shot ? pdFALSE : pdTRUE,
            &t.args_wrp,
            &timer_data::args_wrapper::wrap_func);
    if(t.handler == nullptr)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("xStreamBufferCreate() fail.", error_type::OS_EFAULT);
            OS_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }

    return exit::OK;
}

void timer::set(uint64_t us) OS_NOEXCEPT
{
    timer::us = us;
    if(t.handler && xTimerIsTimerActive( t.handler ))
    {
        xTimerChangePeriod(t.handler, (us / portTICK_PERIOD_MS) / 1'000, portMAX_DELAY);
    }
}

void timer::set_from_isr(uint64_t us) OS_NOEXCEPT
{
    timer::us = us;
    if(t.handler && xTimerIsTimerActive( t.handler ))
    {
        xTimerChangePeriodFromISR(t.handler, (us / portTICK_PERIOD_MS) / 1'000, nullptr);
    }
}

void timer::start() const OS_NOEXCEPT
{
    if(t.handler)
    {
        xTimerStart(t.handler, (us / portTICK_PERIOD_MS) / 1'000);
    }
}

void timer::start_from_isr() const OS_NOEXCEPT
{
    if(t.handler)
    {
        xTimerStartFromISR(t.handler, nullptr);
    }
}

void timer::stop() const OS_NOEXCEPT
{
    if(t.handler && xTimerIsTimerActive( t.handler ))
    {
        xTimerStop(t.handler, portMAX_DELAY);
    }
}

void timer::stop_from_isr() const OS_NOEXCEPT
{
    if(t.handler && xTimerIsTimerActive( t.handler ))
    {
        xTimerStopFromISR(t.handler, nullptr);
    }
}

bool timer::is_active() const OS_NOEXCEPT
{
    return t.handler && xTimerIsTimerActive( t.handler );
}


}
}
