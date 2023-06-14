#include <gtest/gtest.h>

#include"osal/osal.hpp"

TEST(thread_test, base) {

    os::thread* t = os::thread::build( "test", 4, 1024, [](auto args) {});


}
