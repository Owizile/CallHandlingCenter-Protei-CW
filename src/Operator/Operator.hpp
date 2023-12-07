#ifndef OPERATOR_HPP
#define OPERATOR_HPP

#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>
#include <random>
#include "Call.hpp"
#include "CDR.hpp"

class Operator {
public:
    Operator(boost::asio::io_service& ioService_);
    Operator(const Operator& other);

    boost::uuids::uuid getID();
    bool isFree();
    void handleCall(size_t minCallDuration, size_t maxCallDuration, Call* call, CDR* cdr);

    // thinking of using priority_queue, but io_service not able for copy
    // bool operator<(const Operator& other) const {
    //     return (!isBusy && other.isBusy);
    // }

private:
    boost::uuids::uuid ID;

    boost::asio::io_service& ioService;
    boost::asio::deadline_timer callTimer;

    bool isBusy = false;
};

#endif // OPERATOR_HPP
