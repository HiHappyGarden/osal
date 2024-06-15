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
#include <gtest/gtest.h>

#include "osal/osal.hpp"
#include"common_test.hpp"

namespace
{

#define APP_TAG "SEM"

void* thread_runtime(void * arg);

os::semaphore* sem;
os::thread thread1{"Test1", 4, OASL_TASK_HEAP, thread_runtime};
os::thread thread2{"Test1", 4, OASL_TASK_HEAP, thread_runtime};

void* thread_runtime(void * arg)
{


    //wait
    sem->wait(10'000);
    OSAL_LOG_DEBUG(APP_TAG, "Entered...");

    //critical section
    for(uint32_t i = 0; i < 0xFFF; i++);

    //signal
    OSAL_LOG_DEBUG(APP_TAG, "Just Exiting...");
    sem->signal();

    SUCCEED();

    return nullptr;
}


}


TEST(sem_test, two_thread_sync)
{


    sem = new os::semaphore(2);
    if(sem == nullptr)
    {
        OSAL_LOG_ERROR(APP_TAG, "mutex init failed");
        FAIL();
    }

    thread1.create();

    os::us_sleep(os::sec_to_us(1));

    thread2.create();

    os::us_sleep(os::sec_to_us(1));

}
