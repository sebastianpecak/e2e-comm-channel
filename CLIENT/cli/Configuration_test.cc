#include "Configuration.h"
#include <gtest/gtest.h>

TEST(Configuration, empty)
{
    Configuration instance = Configuration();
    instance.ParseCliArguments(0, nullptr);
}
