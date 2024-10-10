#include <gtest/gtest.h>

TEST(MyProgramTest, SampleTest) {
    EXPECT_EQ(1, 1);
    EXPECT_TRUE(true);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}