#include <gtest/gtest.h>
#include "CDR.hpp"

#define CDR_PATH "CDR.txt"

class MockCall : public Call {
public:
    MockCall(std::string callNumber)
        : Call(callNumber) {
    }

    boost::uuids::uuid getID(){
        return boost::uuids::random_generator()();
    }

    boost::uuids::uuid getOperatorID(){
        return boost::uuids::nil_uuid();
    }

    std::string getCallNumber(){
        return "MockCallNumber";
    }

    boost::posix_time::ptime getStartTime(){
        return boost::posix_time::from_iso_string("20230101T120000");
    }

    boost::posix_time::ptime getConnectTime(){
        return boost::posix_time::from_iso_string("20230101T121500");
    }

    boost::posix_time::ptime getEndTime(){
        return boost::posix_time::from_iso_string("20230101T123000");
    }
};

class CDRTest : public ::testing::Test {
protected:
    CDR cdr{CDR_PATH};
    MockCall mockCall{"123"};

    // Function to clear the contents of the CDR file before each test
    void SetUp() override {
        std::ofstream(CDR_PATH, std::ios::trunc).close();
    }
};

TEST_F(CDRTest, WriteCallOK) {
    mockCall.setConnectTime();
    cdr.writeCall(&mockCall, OK);
    std::ifstream inputFile(CDR_PATH);
    std::string fileContent((std::istreambuf_iterator<char>(inputFile)),
                             std::istreambuf_iterator<char>());
    ASSERT_TRUE(fileContent.find("Call status         : OK;") != std::string::npos);
}

TEST_F(CDRTest, WriteCallAlreadyInQueue) {
    cdr.writeCall(&mockCall, alreadyInQueue);
    std::ifstream inputFile(CDR_PATH);
    std::string fileContent((std::istreambuf_iterator<char>(inputFile)),
                             std::istreambuf_iterator<char>());
    ASSERT_TRUE(fileContent.find("Call status         : already in queue;") != std::string::npos);
}

TEST_F(CDRTest, WriteCallTimeout) {
    cdr.writeCall(&mockCall, timeout);
    std::ifstream inputFile(CDR_PATH);
    std::string fileContent((std::istreambuf_iterator<char>(inputFile)),
                             std::istreambuf_iterator<char>());
    ASSERT_TRUE(fileContent.find("Call status         : timeout;") != std::string::npos);
}

TEST_F(CDRTest, WriteCallOverload) {
    cdr.writeCall(&mockCall, overload);
    std::ifstream inputFile(CDR_PATH);
    std::string fileContent((std::istreambuf_iterator<char>(inputFile)),
                             std::istreambuf_iterator<char>());
    ASSERT_TRUE(fileContent.find("Call status         : overload;") != std::string::npos);
}

// Test case for an unknown CallStatus (default case in switch)
TEST_F(CDRTest, WriteCallUnknownStatus) {
    cdr.writeCall(&mockCall, -1);

    std::ifstream inputFile(CDR_PATH);
    ASSERT_TRUE(inputFile.peek() == std::ifstream::traits_type::eof());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}