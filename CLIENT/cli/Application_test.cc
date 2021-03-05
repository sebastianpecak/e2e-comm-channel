#include "Application.h"
#include <gtest/gtest.h>

TEST(Application, runShouldReturnZero)
{
    Application instance = Application(0, nullptr);
    EXPECT_EQ(instance.Run(), 0);
}
