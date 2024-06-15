/***************************************************************************
 *
 * OSAL
 * Copyright (C) 2023/2024 Antonio Salsi <passy.linux@zresa.it>
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

#include "osal/types.hpp"
//#include <FreeRTOS.h>
//#include <semphr.h>
//#include <event_groups.h>
//#include <queue.h>
//#include <stream_buffer.h>

struct QueueDefinition; /* Using old naming convention so as not to break kernel aware debuggers. */
typedef struct QueueDefinition   * QueueHandle_t;
typedef QueueHandle_t SemaphoreHandle_t;

struct EventGroupDef_t;
typedef struct EventGroupDef_t   * EventGroupHandle_t;

struct StreamBufferDef_t;
typedef struct StreamBufferDef_t * StreamBufferHandle_t;

struct tskTaskControlBlock; /* The old naming convention is used to prevent breaking kernel aware debuggers. */
typedef struct tskTaskControlBlock * TaskHandle_t;

struct tmrTimerControl; /* The old naming convention is used to prevent breaking kernel aware debuggers. */
typedef struct tmrTimerControl * TimerHandle_t;

namespace osal
{
inline namespace v1
{

uint64_t tmo_to_ticks(uint64_t ms) OSAL_NOEXCEPT;

class timer;

struct mutex_data
{
    SemaphoreHandle_t handle = nullptr;
};

struct semaphore_data
{
    SemaphoreHandle_t handle = nullptr;
};

struct event_data
{
    EventGroupHandle_t handle = nullptr;
};

struct queue_data
{
    size_t size = 0;
    size_t count = 0;
    QueueHandle_t handle = nullptr;
};

struct stream_buffer_data
{
    StreamBufferHandle_t handle;
};

struct thread_data
{
    struct args_wrapper final
    {
        void* arg = nullptr;
        void* (*fn)(void* arg) = nullptr;

        static void wrap_func(void * arg);
    };

    TaskHandle_t handler = nullptr;
    args_wrapper args_wrp{};
};


struct timer_data
{

    struct args_wrapper final
    {
        class timer* timer = nullptr;
        void* arg = nullptr;
        void* (*fn)(class timer*, void*) = nullptr;

        static void wrap_func( TimerHandle_t xTimer );
    };
    TimerHandle_t handler = nullptr;
    args_wrapper args_wrp{};
};

using tick = uint64_t;


}
}
