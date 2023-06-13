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

#include <cinttypes>

#include "sys/osal_sys.hpp"

#include "error.hpp"
#include "event.hpp"
#include "log.hpp"
#include "mutex.hpp"
#include "queue.hpp"
#include "semaphore.hpp"
#include "thread.hpp"
#include "timer.hpp"

namespace osal
{

void usleep (uint64_t us) OS_NOEXCEPT;
uint64_t get_current_time_us (void) OS_NOEXCEPT;

tick tick_current (void) OS_NOEXCEPT;
tick tick_from_us (uint64_t us) OS_NOEXCEPT;
void tick_sleep (tick tick) OS_NOEXCEPT;


}
