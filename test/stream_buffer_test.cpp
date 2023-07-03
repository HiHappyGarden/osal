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
#include <gtest/gtest.h>

#include "osal/osal.hpp"


#include <string.h>
#include <iostream>
using namespace std;

TEST(buffer_test, filling_and_emptying)
{
    os::stream_buffer stream(20, 1);

    ASSERT_TRUE(stream.send(reinterpret_cast<const uint8_t*>("ciao 1"), 6, 0));

    ASSERT_TRUE(stream.send(reinterpret_cast<const uint8_t*>("ciao 2"), 6, 0));

    ASSERT_TRUE(stream.send(reinterpret_cast<const uint8_t*>("ciao 3"), 6, 0));

    char buffer[12];

    memset(buffer, '\0', sizeof(buffer));
    ASSERT_TRUE(stream.receive(reinterpret_cast<uint8_t*>(buffer), sizeof(buffer) - 1, 0));
    ASSERT_EQ(strncmp(buffer, "ciao 1ciao ", sizeof(buffer)), 0);

    memset(buffer, '\0', sizeof(buffer));
    ASSERT_TRUE(stream.receive(reinterpret_cast<uint8_t*>(buffer), sizeof(buffer) - 1, 0));

    ASSERT_EQ(strncmp(buffer, "2ciao 3", sizeof(buffer)), 0);
}

TEST(buffer_test, buffer_overflow_filling_and_emptying)
{
    os::stream_buffer stream(20, 1);

    ASSERT_TRUE(stream.send(reinterpret_cast<const uint8_t*>("ciao 1"), 6, 0));

    ASSERT_TRUE(stream.send(reinterpret_cast<const uint8_t*>("ciao 2"), 6, 0));

    ASSERT_TRUE(stream.send(reinterpret_cast<const uint8_t*>("ciao 3"), 6, 0));

    ASSERT_TRUE(stream.send(reinterpret_cast<const uint8_t*>("ciao 4"), 6, 0));

    char buffer[12];

    memset(buffer, '\0', sizeof(buffer));
    ASSERT_TRUE(stream.receive(reinterpret_cast<uint8_t*>(buffer), sizeof(buffer) - 1, 0));
    ASSERT_EQ(strncmp(buffer, "ciao 1ciao ", sizeof(buffer)), 0);

    memset(buffer, '\0', sizeof(buffer));
    ASSERT_TRUE(stream.receive(reinterpret_cast<uint8_t*>(buffer), sizeof(buffer) - 1, 0));

    ASSERT_EQ(strncmp(buffer, "2ciao 3ci", sizeof(buffer)), 0);
}


TEST(buffer_test, repetute_sending_and_receiving)
{
    os::stream_buffer stream(20, 1);
    char buffer[12];


    ASSERT_TRUE(stream.send(reinterpret_cast<const uint8_t*>("ciao 1"), 6, 0));
    memset(buffer, '\0', sizeof(buffer));
    ASSERT_TRUE(stream.receive(reinterpret_cast<uint8_t*>(buffer), sizeof(buffer) - 1, 0));
    ASSERT_EQ(strncmp(buffer, "ciao 1", sizeof(buffer)), 0);

    ASSERT_TRUE(stream.send(reinterpret_cast<const uint8_t*>("ciao 2"), 6, 0));
    memset(buffer, '\0', sizeof(buffer));
    ASSERT_TRUE(stream.receive(reinterpret_cast<uint8_t*>(buffer), sizeof(buffer) - 1, 0));
    ASSERT_EQ(strncmp(buffer, "ciao 2", sizeof(buffer)), 0);

    ASSERT_TRUE(stream.send(reinterpret_cast<const uint8_t*>("ciao 3"), 6, 0));
    memset(buffer, '\0', sizeof(buffer));
    ASSERT_TRUE(stream.receive(reinterpret_cast<uint8_t*>(buffer), sizeof(buffer) - 1, 0));
    ASSERT_EQ(strncmp(buffer, "ciao 3", sizeof(buffer)), 0);

    ASSERT_TRUE(stream.send(reinterpret_cast<const uint8_t*>("ciao 4"), 6, 0));
    memset(buffer, '\0', sizeof(buffer));
    ASSERT_TRUE(stream.receive(reinterpret_cast<uint8_t*>(buffer), 3, 0));
    ASSERT_TRUE(stream.receive(reinterpret_cast<uint8_t*>(buffer + 3), 3, 0));
    ASSERT_EQ(strncmp(buffer, "ciao 4", sizeof(buffer)), 0);
}

TEST(buffer_test, rotation_receive)
{
    os::stream_buffer stream(10, 1);
    char buffer[12];

    ASSERT_TRUE(stream.send(reinterpret_cast<const uint8_t*>("ciao 1"), 6, 0));
    memset(buffer, '\0', sizeof(buffer));
    ASSERT_TRUE(stream.receive(reinterpret_cast<uint8_t*>(buffer), 3, 0));
    ASSERT_EQ(strncmp(buffer, "cia", sizeof(buffer)), 0);


    ASSERT_TRUE(stream.send(reinterpret_cast<const uint8_t*>("ciao 2"), 6, 0));
    memset(buffer, '\0', sizeof(buffer));
    ASSERT_TRUE(stream.receive(reinterpret_cast<uint8_t*>(buffer), 4, 0));
    ASSERT_EQ(strncmp(buffer, "o 1c", sizeof(buffer)), 0);

    memset(buffer, '\0', sizeof(buffer));
    ASSERT_TRUE(stream.receive(reinterpret_cast<uint8_t*>(buffer), 3, 0));
    ASSERT_EQ(strncmp(buffer, "iao", sizeof(buffer)), 0);

    memset(buffer, '\0', sizeof(buffer));
    ASSERT_TRUE(stream.receive(reinterpret_cast<uint8_t*>(buffer), 6, 0));
    ASSERT_EQ(strncmp(buffer, " 2", sizeof(buffer)), 0);

}


TEST(buffer_test, rotation_receive_long)
{
    os::stream_buffer stream(10, 1);
    char buffer[12];

    ASSERT_TRUE(stream.send(reinterpret_cast<const uint8_t*>("ciao 1"), 6, 0));
    memset(buffer, '\0', sizeof(buffer));
    ASSERT_TRUE(stream.receive(reinterpret_cast<uint8_t*>(buffer), 3, 0));
    ASSERT_EQ(strncmp(buffer, "cia", sizeof(buffer)), 0);


    ASSERT_TRUE(stream.send(reinterpret_cast<const uint8_t*>("ciao 2"), 6, 0));
    memset(buffer, '\0', sizeof(buffer));
    ASSERT_TRUE(stream.receive(reinterpret_cast<uint8_t*>(buffer), sizeof(buffer) - 1, 0));
    ASSERT_EQ(strncmp(buffer, "o 1ciao 2", sizeof(buffer)), 0);

}

TEST(buffer_test, rotation_send)
{
    os::stream_buffer stream(10, 1);
    char buffer[12];


    ASSERT_TRUE(stream.send(reinterpret_cast<const uint8_t*>("ciao 1"), 6, 0));
    memset(buffer, '\0', sizeof(buffer));
    ASSERT_TRUE(stream.receive(reinterpret_cast<uint8_t*>(buffer), 3, 0));
    ASSERT_EQ(strncmp(buffer, "cia", sizeof(buffer)), 0);


    ASSERT_TRUE(stream.send(reinterpret_cast<const uint8_t*>("ciao 2"), 6, 0));
    memset(buffer, '\0', sizeof(buffer));
    ASSERT_TRUE(stream.receive(reinterpret_cast<uint8_t*>(buffer), 4, 0));
    ASSERT_EQ(strncmp(buffer, "o 1c", sizeof(buffer)), 0);

    ASSERT_TRUE(stream.send(reinterpret_cast<const uint8_t*>("----"), 4, 0));

    memset(buffer, '\0', sizeof(buffer));
    ASSERT_TRUE(stream.receive(reinterpret_cast<uint8_t*>(buffer), 3, 0));
    ASSERT_EQ(strncmp(buffer, "iao", sizeof(buffer)), 0);

    memset(buffer, '\0', sizeof(buffer));
    ASSERT_TRUE(stream.receive(reinterpret_cast<uint8_t*>(buffer), 6, 0));
    ASSERT_EQ(strncmp(buffer, " 2----", sizeof(buffer)), 0);

}

TEST(buffer_test, is_full_is_empty_size)
{
    os::stream_buffer stream(10, 1);
    char buffer[12];


    ASSERT_EQ(stream.send(reinterpret_cast<const uint8_t*>("1234567890"), 10, 0), 10);

    ASSERT_TRUE(stream.is_full());

    memset(buffer, '\0', sizeof(buffer));
    ASSERT_EQ(stream.receive(reinterpret_cast<uint8_t*>(buffer), 5, 0), 5);
    ASSERT_EQ(strncmp(buffer, "12345", sizeof(buffer)), 0);

    ASSERT_EQ(stream.size(), 5);

    memset(buffer, '\0', sizeof(buffer));
    ASSERT_EQ(stream.receive(reinterpret_cast<uint8_t*>(buffer), 5, 0), 5);
    ASSERT_EQ(strncmp(buffer, "67890", sizeof(buffer)), 0);

    ASSERT_EQ(stream.bytes_free(), 10);

    ASSERT_TRUE(stream.is_empty());
}


TEST(buffer_test, full_read)
{
    os::stream_buffer stream(10, 1);
    char buffer[12];

    ASSERT_TRUE(stream.send(reinterpret_cast<const uint8_t*>("1234567890"), 10, 0));
    memset(buffer, '\0', sizeof(buffer));
    ASSERT_TRUE(stream.receive(reinterpret_cast<uint8_t*>(buffer), 10, 0));
    ASSERT_EQ(strncmp(buffer, "1234567890", sizeof(buffer)), 0);

}

TEST(buffer_test, rotation_full_read)
{
    os::stream_buffer stream(10, 1);
    char buffer[11];

    ASSERT_TRUE(stream.send(reinterpret_cast<const uint8_t*>("1234567890"), 10, 0));
    memset(buffer, '\0', sizeof(buffer));
    ASSERT_TRUE(stream.receive(reinterpret_cast<uint8_t*>(buffer), 5, 0));
    ASSERT_EQ(strncmp(buffer, "12345", sizeof(buffer)), 0);

    ASSERT_TRUE(stream.send(reinterpret_cast<const uint8_t*>("ABCDE"), 5, 0));
    memset(buffer, '\0', sizeof(buffer));
    ASSERT_TRUE(stream.receive(reinterpret_cast<uint8_t*>(buffer), 10, 0));
    ASSERT_EQ(strncmp(buffer, "67890ABCDE", sizeof(buffer)), 0);
}

string now() {
    time_t t = time(0);
    char buffer[9] = {0};

    strftime(buffer, 9, "%H:%M:%S", localtime(&t));
    return string(buffer);
}

namespace
{
os::stream_buffer stream(10, 1);
uint8_t count_bytes = 0;
bool run = true;
}

TEST(buffer_test, task_wait)
{
    os::thread thread1{"thread1", 4, 1024, [](void * arg) -> void*
                       {
                           while(run)
                           {
                               uint8_t buffer[12];
                               stream.receive(buffer, sizeof(buffer), os::WAIT_FOREVER);
                               cout << "thread1" << endl;
                               run = false;
                               //ASSERT_EQ(count_bytes, 4);
                           }
                           return nullptr;
                       }
    };
    os::thread thread2{"thread2"
                       , 4
                       , 1024
                       , [](void * arg) -> void*
                       {
                           count_bytes = 0;
                           while(run)
                           {

                               stream.send(&count_bytes, 1, 0);
                               cout << "thread2: " << now() << endl;

                               count_bytes++;
                               if(count_bytes > 9)
                               {
                                   count_bytes = 0;
                               }
                               os::tick_sleep(os::sec_to_us(1));
                           }
                           return nullptr;
                       }};

    char buffer[12];

    memset(buffer, '\0', sizeof(buffer));

    bool status;

    status = thread1.create(nullptr);

    status = thread2.create(nullptr);

     os::tick_sleep(os::sec_to_us(6));

    ASSERT_EQ(count_bytes, 4);
}
