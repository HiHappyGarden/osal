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
