#include <gtest/gtest.h>

#include"osal/osal.hpp"


TEST(error_test, base)
{

    os::error err = OSAL_BUILD_ERROR("test", 20);


    ASSERT_TRUE(strcmp(err.get_msg(), "test") == 0);
    ASSERT_TRUE(strcmp(err.get_file(), "error_test.cpp") == 0);
    ASSERT_TRUE(strcmp(err.get_func(), "TestBody") == 0);
    ASSERT_TRUE(err.get_line() == 9);

}


TEST(error_test, old_error)
{

    os::error old = OSAL_BUILD_ERROR("old", 20);


    os::error err(old, "test",30, osal::get_file_name(__FILE__), __FUNCTION__, __LINE__);

    os::printf_stack_error(err);

}

