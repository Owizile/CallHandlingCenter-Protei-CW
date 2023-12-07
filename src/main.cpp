#include <iostream>
#include "CallCenter.hpp"

int main() {
    try {
        boost::asio::io_service io_service;
        CallCenter callCenter(io_service, 8081);  // set desired port
        io_service.run();
    } catch (const std::exception& e) {
        BOOST_LOG_TRIVIAL(fatal) << "Exception: " << e.what();
    }

    return 0;
}
