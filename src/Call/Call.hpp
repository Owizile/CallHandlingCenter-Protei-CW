#ifndef CALL_HPP
#define CALL_HPP

#include <string>
#include <boost/date_time.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

class Call
{
private:
    boost::uuids::uuid ID;
    boost::uuids::uuid operatorID;

    std::string callNumber;

    boost::posix_time::ptime startTime;
    boost::posix_time::ptime connectTime;
    boost::posix_time::ptime endTime;

public:
    Call(std::string callNumber_);

    boost::uuids::uuid getID();
    boost::uuids::uuid getOperatorID();
    std::string getCallNumber();
    boost::posix_time::ptime getStartTime();
    boost::posix_time::ptime getConnectTime();
    boost::posix_time::ptime getEndTime();

    void setEndTime(boost::posix_time::ptime  endTime_ = boost::posix_time::ptime());
    void setConnectTime(boost::posix_time::ptime  connectTime_ = boost::posix_time::ptime());
    void setOperatorID(boost::uuids::uuid opID);
};

#endif // CALL_HPP
