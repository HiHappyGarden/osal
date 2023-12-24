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

#include"osal/osal.hpp"
#include"common_test.hpp"


// Global variable to store the shared value
int shared_value = 0;

// Mutex to synchronize access to shared_value
os::mutex mutex;

constexpr const char APP_TAG[]  = "MUTEX TEST";

// Function to be executed by the threads
void* thread_function(void*)
{
    // Acquire the lock
    mutex.lock();

    // Increment the shared value
    shared_value++;

    // Release the lock
    mutex.unlock();

    pthread_exit(nullptr);
}

// Test case for concurrent increment
TEST(thread_test, increment_test)
{
    const int num_threads = 10;

    // Create an array of threads
    os::thread* threads[num_threads];

    // Initialize the mutex
    //pthread_mutex_init(&mutex, nullptr);

    // Create the threads
    for (int i = 0; i < num_threads; ++i) {
        os::error* e = nullptr;
        threads[i] = new os::thread("test", 1, OASL_TASK_HEAP, thread_function);

        if(threads[i]->create(nullptr, &e) == osal::exit::KO)
        {
            os::printf_stack_error(APP_TAG, e);
        }
    }

    os::us_sleep(1'000'000);

    // Check the final value of the shared variable
    EXPECT_EQ(shared_value, num_threads);
}
