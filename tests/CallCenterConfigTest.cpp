#include <gtest/gtest.h>
#include <filesystem>  // For std::filesystem::remove
#include "CallCenterConfig.hpp"

class CallCenterConfigTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a temporary config file for testing
        configFile.open(tempConfigPath, std::ios::out);
        configFile << R"(
        {
            "queueMaxSize": 100,
            "holdMinDuration": 5,
            "holdMaxDuration": 10,
            "callMinDuration": 15,
            "callMaxDuration": 30,
            "operatorsNum": 3
        }
        )";
        configFile.close();
    }

    void TearDown() override {
        // Remove the temporary config file after testing
        std::filesystem::remove(tempConfigPath);
    }

    CallCenterConfig config;
    std::fstream configFile;
    const std::string tempConfigPath = "temp_config.json";
};

TEST_F(CallCenterConfigTest, InitializeFromFile) {
    // Expectation: The configuration should be loaded successfully from the temporary file

    config.initialize(tempConfigPath);

    EXPECT_EQ(config.queueMaxSize, 100);
    EXPECT_EQ(config.holdMinDuration, 5);
    EXPECT_EQ(config.holdMaxDuration, 10);
    EXPECT_EQ(config.callMinDuration, 15);
    EXPECT_EQ(config.callMaxDuration, 30);
    EXPECT_EQ(config.operatorsNum, 3);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}