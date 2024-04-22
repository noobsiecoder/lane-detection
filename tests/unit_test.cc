#include <gtest/gtest.h>
#include "greet.h"

TEST(GreetTest, SayHello)
{
    ASSERT_EQ(sayHello(), "Hello, World!") << "Not Equal";
}