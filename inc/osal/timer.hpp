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
#pragma once


#include "osal/error.hpp"
#include "osal_sys/osal_sys.hpp"

#include <stdint.h>

namespace osal
{
inline namespace v1
{

class timer final
{

public:

    /**
     * @typedef handler
     * @brief Function pointer type for the thread handler.
     * @param arg The argument passed to the thread handler.
     */
    using handler = void* (*)(timer*, void*);

    timer(uint64_t us, handler fn, bool oneshot = false) OS_NOEXCEPT;
    timer(const timer&) = delete;
    timer& operator = (const timer&) = delete;
    timer(timer&&) = delete;
    timer& operator = (timer&&) = delete;
    ~timer() OS_NOEXCEPT;

    bool create(void * arg = nullptr, error** error = nullptr) OS_NOEXCEPT;

    void set(uint64_t us) OS_NOEXCEPT;

    void start() OS_NOEXCEPT;

    void start_from_isr() OS_NOEXCEPT;

    void stop() OS_NOEXCEPT;

    void stop_from_isr() OS_NOEXCEPT;

private:
    uint64_t us;
    handler fn;
    bool oneshot;

    timer_data t;

    friend void* timer_thread (void *);
};


}
}

