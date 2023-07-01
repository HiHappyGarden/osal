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


#define OS_THREAD_DATA
#define OS_MUTEX_DATA
#define OS_SEM_DATA
#define OS_EVENT_DATA
#define OS_MBOX_DATA
#define OS_TIMER_DATA
#define OS_TICK_DATA
#define OS_STREAM_DATA


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
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    size_t count;
};

struct event_data
{
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    uint32_t flags;
};

struct queue_data
{
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    size_t r;
    size_t w;
    size_t count;
    size_t size;
    void * msg[];
};

struct timer_data
{
    timer_t timerid;
    //thread* thread;
    pid_t thread_id;
    bool exit;
    void (*fn) (class timer *, void*);
    void * arg;
    uint32_t us;
    bool oneshot;
};

struct stream_data
{
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    size_t trigger_size;
    size_t r;
    size_t w;
    size_t end;
    size_t count;
    size_t size;
    uint8_t* buffer;
};

struct stream
{
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    size_t trigger_size;
    size_t r;
    size_t w;
    size_t end;
    size_t count;
    size_t size;
    uint8_t* buffer;
};

using tick = uint64_t;


}
}
