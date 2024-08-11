#include <boost/asio.hpp>
#include "Client.hpp"

void runClient(boost::asio::io_service& io_service)
{
    io_service.run();
}

int main() {
    try {
        boost::asio::io_service io_service;
        auto client = std::make_shared<Client>(io_service);
        client->connect();
        std::thread io_thread(&runClient, std::ref(io_service));
        io_thread.detach();
        std::string message;
        while (std::getline(std::cin, message)) {
            client->sendMessage(message);
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
