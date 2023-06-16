#include <gtest/gtest.h>

#include"osal/osal.hpp"

namespace
{

bool check = false;

#ifdef __MACH__
#include <limits.h>
void * _Nullable thread_test(void * _Nullable)
{
    check = true;
    return nullptr;
}
#else
void * thread_test(void *)
{
    check = true;
    return nullptr;
}
#endif
}


TEST(thread_test, base)
{
    os::thread* t = os::thread::build( "test", 4, 1024, thread_test);

    t->create();

    os::us_sleep(1'000'000);

    ASSERT_TRUE(check);

    delete t;
}

