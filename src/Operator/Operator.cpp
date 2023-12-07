#include "Operator.hpp"

size_t getRandomNumberInRange(size_t num1, size_t num2) {
    size_t range = num2 - num1 + 1;
    return num1 + rand() % range;
}

Operator::Operator(boost::asio::io_service& ioService_) 
    : ioService(ioService_), callTimer(ioService_) {
    ID = boost::uuids::random_generator()();
}

Operator::Operator(const Operator& other)
    : ioService(other.ioService), callTimer(ioService) {
    ID = other.ID;
    isBusy = other.isBusy;
}

void Operator::handleCall(size_t minCallDuration, size_t maxCallDuration, Call* call, CDR* cdr) {
    size_t handleTime = getRandomNumberInRange(minCallDuration, maxCallDuration);

    call->setOperatorID(getID());
    call->setConnectTime();

    isBusy = true;

    callTimer.expires_from_now(boost::posix_time::seconds(handleTime));

    callTimer.async_wait([this, call, cdr](const boost::system::error_code& error) {
        if (!error) {
            isBusy = false;
            call->setEndTime();
            cdr->writeCall(call, OK);
            BOOST_LOG_TRIVIAL(info) << "Operator " << ID 
                                    << " completed call " << call->getID()
                                    << " call number: " << call->getCallNumber();
            delete call;
        } else {
            BOOST_LOG_TRIVIAL(error) << "Async wait error in call handling: " << error.message();
        }
    });
}

bool Operator::isFree() { return !isBusy; }

boost::uuids::uuid Operator::getID() { return ID; }
