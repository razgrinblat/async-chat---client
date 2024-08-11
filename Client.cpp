#include "Client.hpp"

Client::Client(boost::asio::io_service& io_service)
    : _socket(io_service), _resolver(io_service){
}


void Client::connect() 
{
    auto endpoints = _resolver.resolve(HOST, std::to_string(PORT));
    boost::asio::async_connect(_socket, endpoints, [this](const boost::system::error_code& error, const boost::asio::ip::tcp::endpoint& endpoint) {
        if (!error) {
            std::cout << "Connected to : " << endpoint.address() << " ,port: " << endpoint.port() << std::endl;
            startReading();
        }
        else {
            std::cerr << "Error connecting to server: " << error.message() << std::endl;
        }
        });
}

void Client::startReading() 
{
    
    _socket.async_receive(boost::asio::buffer(_buffer), [this](const boost::system::error_code& error, std::size_t bytes_transferred) 
        {
        handleRead(error, bytes_transferred);
        });
}

void Client::handleRead(const boost::system::error_code& error, std::size_t bytes_transferred) {
    if (!error) {
        std::cout << "Received message: " << std::string(_buffer.data(), bytes_transferred) << std::endl;
        startReading();
    }
    else {
        std::cerr << "Error reading from server: " << error.message() << std::endl;
    }
}

void Client::sendMessage(const std::string& message) 
{
    auto message_ptr = std::make_shared<std::string>(message);
    boost::asio::async_write(_socket, boost::asio::buffer(*message_ptr), [message_ptr](const boost::system::error_code& error, std::size_t bytes) {
        if (error) 
        {
            std::cerr << "Error sending message: " << error.message() << std::endl;
        }
        });
}
