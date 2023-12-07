#include <gtest/gtest.h>
#include "CallQueue.hpp"

// Mock class for the CDR class
class MockCDR : public CDR {
public:
    MockCDR() : CDR("") {} // Pass an empty string as the path for simplicity
};

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

// Test fixture for CallQueue tests
class CallQueueTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockCDR = new MockCDR;
        callQueue = new CallQueue(mockCDR);
    }

    void TearDown() override {
        delete callQueue;
        delete mockCDR;
    }

    CallQueue* callQueue;
    MockCDR* mockCDR;
};

// Test case for adding a call to the queue
TEST_F(CallQueueTest, AddCall) {
    std::string callNumber = "123";
    MockCall* mockCall = new MockCall(callNumber);

    callQueue->addCall(mockCall, std::time(nullptr), 10);

    EXPECT_EQ(callQueue->getSize(), 1);

    EXPECT_TRUE(callQueue->isCallNumberInQueue(callNumber));
}

// Test case for removing an expired call from the queue
TEST_F(CallQueueTest, RemoveExpiredCall) {
    std::string callNumber ="456";
    MockCall* mockCall = new MockCall(callNumber);

    callQueue->addCall(mockCall, std::time(nullptr), 0);

    EXPECT_EQ(callQueue->getSize(), 1);

    // TODO: think how to get rid of a blocking call
    usleep(100000);

    EXPECT_EQ(callQueue->getSize(), 0);
    EXPECT_FALSE(callQueue->isCallNumberInQueue(callNumber));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}