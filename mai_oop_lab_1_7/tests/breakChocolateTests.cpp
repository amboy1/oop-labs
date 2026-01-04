#include <gtest/gtest.h>
#include "../src/breakChocolate.h"

TEST(BreakChocolateTest, PositiveCase) {
    EXPECT_EQ(breakChocolate(3, 4), 11);
    EXPECT_EQ(breakChocolate(1, 1), 0);
    EXPECT_EQ(breakChocolate(5, 5), 24);
}

TEST(BreakChocolateTest, EdgeCase) {
    EXPECT_EQ(breakChocolate(1, 0), 0);
    EXPECT_EQ(breakChocolate(0, 0), 0);
}