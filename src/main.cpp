#include <iostream>
#include "CallCenter.hpp"

int main() {
    try {
        int port = 8080; // set desired port
        BOOST_LOG_TRIVIAL(info) << "Starting CallCenter on port: " << port;

        boost::asio::io_service io_service;
        CallCenter callCenter(io_service, port);
        io_service.run();
    } catch (const std::exception& e) {
        BOOST_LOG_TRIVIAL(fatal) << "Exception: " << e.what();
    }

    return 0;
}
