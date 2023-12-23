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
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>

namespace osal
{
inline namespace v1
{

class thread;
class timer;

using thread_data = pthread_t;
using mutex_data = pthread_mutex_t;

struct semaphore_data
{
    pthread_cond_t cond{};
    pthread_mutex_t mutex{};
    size_t count = 0;
};

struct event_data
{
    pthread_cond_t cond{};
    pthread_mutex_t mutex{};
    uint32_t flags = 0;
};

struct queue_data
{
    pthread_cond_t cond{};
    pthread_mutex_t mutex{};
    size_t r = 0;
    size_t w = 0;
    size_t count = 0;
    size_t size = 0;
    size_t message_size = 0;
    uint8_t* msg = nullptr;
    size_t buffer_size = 0;
};


struct stream_buffer_data
{
    pthread_cond_t cond{};
    pthread_mutex_t mutex{};
    size_t trigger_size{};
    size_t r = 0;
    size_t w = 0;
    size_t end = 0;
    size_t count = 0;
    size_t size = 0;
    uint8_t* buffer = nullptr;
};


struct timer_data
{
    timer_t timer_id{};
    class thread* thread = nullptr;
    pid_t thread_id{};
    bool exit = false;
    void* (*fn) (class timer*, void*) = nullptr;
    void* arg = nullptr;
    uint32_t us = 0;
    bool one_shot = true;
};

using tick = uint64_t;


}
}
