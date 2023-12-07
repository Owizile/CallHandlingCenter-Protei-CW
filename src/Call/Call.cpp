#include "Call.hpp"

Call::Call(std::string callNumber_) {
    callNumber = callNumber_;
    ID = boost::uuids::random_generator()();
    boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
    startTime = now;
}

void Call::setEndTime(boost::posix_time::ptime endTime_) {
    boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
    endTime_.is_not_a_date_time() ? endTime = now 
                                  : endTime = endTime_;
}

void Call::setConnectTime(boost::posix_time::ptime  connectTime_) {
    boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
    connectTime_.is_not_a_date_time() ? connectTime = now
                                      : connectTime = connectTime_;
}

void Call::setOperatorID(boost::uuids::uuid opID) {
    operatorID = opID;
}

boost::uuids::uuid Call::getID() { return ID; }
boost::uuids::uuid Call::getOperatorID() { return operatorID; }
boost::posix_time::ptime Call::getStartTime() { return startTime; }
boost::posix_time::ptime Call::getConnectTime() { return connectTime; }
boost::posix_time::ptime Call::getEndTime() { return endTime; }
std::string Call::getCallNumber() { return callNumber; }
