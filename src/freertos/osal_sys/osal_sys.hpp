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

#include <FreeRTOS.h>
#include <semphr.h>
#include <event_groups.h>
#include <queue.h>
#include <stream_buffer.h>

namespace osal
{
inline namespace v1
{

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

};

struct thread_data
{

};


struct timer_data
{

};

using tick = uint64_t;


}
}
