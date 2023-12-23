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

#include <FreeRTOS.h>
#include <time.h>
#include <signal.h>

namespace osal
{
inline namespace v1
{


void us_sleep (uint64_t us) OS_NOEXCEPT
{
    vTaskDelay ((us / portTICK_PERIOD_MS) / 1'000);
}

[[maybe_unused]] uint64_t get_current_time_us () OS_NOEXCEPT
{
    return 1'000 * (xTaskGetTickCount() / portTICK_PERIOD_MS);
}

tick tick_current () OS_NOEXCEPT
{
    return xTaskGetTickCount();
}

tick tick_from_us (uint64_t us) OS_NOEXCEPT
{
    return us / (1'000u * portTICK_PERIOD_MS);
}

void tick_sleep (tick tick) OS_NOEXCEPT
{
    vTaskDelay (tick);
}

void set_sleep_main_loop(uint16_t sleep_timing) OS_NOEXCEPT { }

uint16_t get_sleep_main_loop() OS_NOEXCEPT
{
    return 0;
}

void set_check_main_loop(bool check) OS_NOEXCEPT { }

void start_main_loop() OS_NOEXCEPT
{
    vTaskStartScheduler();
}

void stop_main_loop() OS_NOEXCEPT
{
    vTaskEndScheduler();
}

#ifndef EXCLUE_CHECK_FOR_STACK_OVERFLOW
extern "C" [[noreturn]] void vApplicationStackOverflowHook( TaskHandle_t xTask, char * pcTaskName )
{
    (void)xTask;
    (void)pcTaskName;
    for(;;);
}
#endif

#ifndef EXCLUE_USE_MALLOC_FAILED_HOOK
extern "C" [[noreturn]] void vApplicationMallocFailedHook( void )
{
    for(;;);
}
#endif

}
}
