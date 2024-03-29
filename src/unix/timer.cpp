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

#include <signal.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <unistd.h>


namespace osal
{
inline namespace v1
{

namespace
{
constexpr inline const uint8_t TIMER_PRIO = 30;
constexpr inline const uint32_t TIMER_HEAP = 1024;
}

void* timer_thread (void * arg)
{
    auto timer = static_cast<class timer*>(arg);
    sigset_t sigset{0};
    siginfo_t si{0};
    timespec tmo{0};

    timer->t.thread_id = (pid_t)syscall (SYS_gettid);

    /* Add SIGALRM */
    sigemptyset (&sigset);
    sigprocmask (SIG_BLOCK, &sigset, nullptr);
    sigaddset (&sigset, SIGALRM);

    tmo.tv_sec  = 0;
    tmo.tv_nsec = 500'000'000;

    while (!timer->t.exit)
    {
        uint8_t sig = sigtimedwait (&sigset, &si, &tmo);
        if (sig == SIGALRM)
        {
            if (timer->t.fn)
                timer->t.fn (timer, timer->t.arg);
        }
    }
    return nullptr;
}



timer::timer(uint64_t us, handler fn, bool one_shot) OS_NOEXCEPT
    : us(us)
    , fn(fn)
    , one_shot(one_shot)
{

}

timer::~timer() OS_NOEXCEPT
{
    t.exit = true;
    //pthread_join (timer->thread, nullptr);
    t.thread->join();
    delete t.thread;
    t.thread = nullptr;
    timer_delete (t.timer_id);
}

exit timer::create(void *arg, error** error)
{
    sigevent sev{0};
    sigset_t sigset{0};

    /* Block SIGALRM in calling thread */
    sigemptyset (&sigset);
    sigaddset (&sigset, SIGALRM);
    sigprocmask (SIG_BLOCK, &sigset, nullptr);

    t.exit      = false;
    t.thread_id = 0;
    t.fn        = fn;
    t.arg       = arg;
    t.us        = us;
    t.one_shot   = one_shot;

    /* Create timer thread */
    //kiwi_os_thread_create (&t.thread, "os_timer", TIMER_PRIO, 1024, os_timer_thread, this);
    t.thread = new thread("os_timer", TIMER_PRIO, TIMER_HEAP, timer_thread);
    if(t.thread == nullptr)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("Out off memory.", error_type::OS_ENOMEM);
            OS_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }
    if(t.thread->create(this) == exit::KO)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("Impossible create timer.", error_type::OS_ENODATA);
            OS_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }

    /* Wait until timer thread sets its (kernel) thread id */
    do
    {
        sched_yield();
    } while (t.thread_id == 0);

    /* Create timer */
    sev.sigev_notify            = SIGEV_THREAD_ID;
    sev.sigev_value.sival_ptr   = this;
    sev._sigev_un._tid          = t.thread_id;
    sev.sigev_signo             = SIGALRM;
    sev.sigev_notify_attributes = nullptr;

    if (timer_create (CLOCK_MONOTONIC, &sev, &t.timer_id) == -1)
    {
        // free (timer);
        return exit::KO;
    }
    return exit::OK;
}

inline void timer::set(uint64_t us) OS_NOEXCEPT
{
    t.us = us;
}

inline void timer::set_from_isr(uint64_t us) OS_NOEXCEPT
{
    timer::us = us;
}

void timer::start() const OS_NOEXCEPT
{
    itimerspec its{0};


    /* Start timer */
    its.it_value.tv_sec     = 0;
    its.it_value.tv_nsec    = 1000 * t.us;
    its.it_interval.tv_sec  = (t.one_shot) ? 0 : its.it_value.tv_sec;
    its.it_interval.tv_nsec = (t.one_shot) ? 0 : its.it_value.tv_nsec;
    timer_settime (t.timer_id, 0, &its, nullptr);
}

inline  void timer::start_from_isr() const OS_NOEXCEPT
{
    start();
}

void timer::stop() const OS_NOEXCEPT
{
    itimerspec its {0};

    /* Stop timer */
    its.it_value.tv_sec     = 0;
    its.it_value.tv_nsec    = 0;
    its.it_interval.tv_sec  = 0;
    its.it_interval.tv_nsec = 0;
    timer_settime (t.timer_id, 0, &its, nullptr);
}

inline void timer::stop_from_isr() const OS_NOEXCEPT
{
    stop();
}

inline bool timer::is_active() const OS_NOEXCEPT
{
    return !t.exit;
}

}
}
