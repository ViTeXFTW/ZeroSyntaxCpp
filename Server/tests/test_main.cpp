#include <gtest/gtest.h>
#include <gmock/gmock.h>

int main(int argc, char** argv) {
    // Initialize Google Test
    ::testing::InitGoogleTest(&argc, argv);
    
    // Run the tests
    return RUN_ALL_TESTS();
}