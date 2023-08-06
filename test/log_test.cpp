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


#define APP_TAG "TEST"

TEST(log_test, base)
{
    OS_LOG_DEBUG(APP_TAG, "debug");
    OS_LOG_INFO(APP_TAG, "info");
    OS_LOG_WARNING(APP_TAG, "warning");
    OS_LOG_ERROR(APP_TAG, "error");
    OS_LOG_FATAL(APP_TAG, "fatal");
    os::reset_log_color();
}


TEST(log_test, set_level)
{

    os::set_level_log(os::LEVEL_INFO);

    OS_LOG_DEBUG(APP_TAG, "debug");
    OS_LOG_INFO(APP_TAG, "info");
    OS_LOG_WARNING(APP_TAG, "warning");
    OS_LOG_ERROR(APP_TAG, "error");
    OS_LOG_FATAL(APP_TAG, "fatal");
    os::reset_log_color();
}


TEST(log_test, disable_log)
{

    os::set_enable_log(false);

    OS_LOG_DEBUG(APP_TAG, "debug");
    OS_LOG_INFO(APP_TAG, "info");
    OS_LOG_WARNING(APP_TAG, "warning");
    OS_LOG_ERROR(APP_TAG, "error");
    OS_LOG_FATAL(APP_TAG, "fatal");
    os::reset_log_color();
}

TEST(log_test, builder)
{
    osal::error* error;

}
