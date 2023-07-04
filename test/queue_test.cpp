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

#include"osal/osal.hpp"

#include <stdio.h>

#define APP_TAG "queue"

TEST(queue_test, single_thread)
{
    struct test_data_type
    {
        uint32_t count;
        uint32_t field2;
    }test_data = {0};

    os::queue mbox{10, sizeof(struct test_data_type)};

    test_data.count = 0;
    test_data.field2 = 10;
    if(mbox.post((uint8_t *)&test_data, 0))
    {
        OS_LOG_INFO(APP_TAG, "count:%u insert: %u", test_data.count, test_data.field2);
    }
    else
    {
        OS_LOG_ERROR(APP_TAG, "os_mbox_post error");
    }
    EXPECT_EQ(mbox.size(), 1);

    test_data.count = 1;
    test_data.field2 = 20;
    if(mbox.post((uint8_t *)&test_data, os::WAIT_FOREVER))
    {
        OS_LOG_INFO(APP_TAG, "count:%u insert: %u", test_data.count, test_data.field2);
    }
    else
    {
        OS_LOG_ERROR(APP_TAG, "os_mbox_post error");
    }
    EXPECT_EQ(mbox.size(), 2);

    test_data.count = 2;
    test_data.field2 = 30;
    if(mbox.post((uint8_t *)&test_data, os::WAIT_FOREVER))
    {
        OS_LOG_INFO(APP_TAG,  "count:%u insert: %u", test_data.count, test_data.field2);
    }
    else
    {
        OS_LOG_ERROR(APP_TAG, "os_mbox_post error");
    }
    EXPECT_EQ(mbox.size(), 3);


    struct test_data_type data = {0};
    if(mbox.fetch(&data, 0))
    {
        OS_LOG_INFO(APP_TAG, "count:%d read:%u", data.count, data.field2);
        EXPECT_EQ(data.count, 0);
        EXPECT_EQ(data.field2, 10);

    }
    else
    {
        OS_LOG_ERROR(APP_TAG, "os_mbox_fetch error");
    }
    EXPECT_EQ(mbox.size(), 2);

    if(mbox.fetch(&data, os::WAIT_FOREVER))
    {
        OS_LOG_INFO(APP_TAG,  "count:%d read:%u", data.count, data.field2);
        EXPECT_EQ(data.count, 1);
        EXPECT_EQ(data.field2, 20);
    }
    else
    {
         OS_LOG_ERROR(APP_TAG, "os_mbox_fetch error");
    }
    EXPECT_EQ(mbox.size(), 1);

    if(mbox.fetch(&data, os::WAIT_FOREVER))
    {
        OS_LOG_INFO(APP_TAG, "count:%d read:%u", data.count, data.field2);
        EXPECT_EQ(data.count, 2);
        EXPECT_EQ(data.field2, 30);
    }
    else
    {
        OS_LOG_ERROR(APP_TAG,"os_mbox_fetch error");
    }
    EXPECT_EQ(mbox.size(), 0);

}
