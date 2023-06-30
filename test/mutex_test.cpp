/***************************************************************************
 *
 * PROJECT
 * Copyright (C) 202X  Antonio Salsi <passy.linux@zresa.it>
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


// Global variable to store the shared value
int shared_value = 0;

// Mutex to synchronize access to shared_value
pthread_mutex_t mutex;

// Function to be executed by the threads
void* thread_function(void*) {
    // Acquire the lock
    pthread_mutex_lock(&mutex);

    // Increment the shared value
    shared_value++;

    // Release the lock
    pthread_mutex_unlock(&mutex);

    pthread_exit(nullptr);
}

// Test case for concurrent increment
TEST(ConcurrencyTest, IncrementTest) {
    const int num_threads = 10;

    // Create an array of threads
    pthread_t threads[num_threads];

    // Initialize the mutex
    pthread_mutex_init(&mutex, nullptr);

    // Create the threads
    for (int i = 0; i < num_threads; ++i) {
        int rc = pthread_create(&threads[i], nullptr, thread_function, nullptr);
        ASSERT_EQ(rc, 0) << "Failed to create thread";
    }

    // Wait for the threads to finish
    for (int i = 0; i < num_threads; ++i) {
        int rc = pthread_join(threads[i], nullptr);
        ASSERT_EQ(rc, 0) << "Failed to join thread";
    }

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);

    // Check the final value of the shared variable
    EXPECT_EQ(shared_value, num_threads);
}
