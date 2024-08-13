#include <boost/asio.hpp>
#include "Client.hpp"



int main() {
    try {
        boost::asio::io_service io_service;
        
        Client client(io_service);
        client.openClient(io_service);
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
