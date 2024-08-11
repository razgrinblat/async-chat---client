#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <array>
#include <thread>

class Client : public std::enable_shared_from_this<Client> 
{
private:
    
    boost::asio::ip::tcp::socket _socket;
    boost::asio::ip::tcp::resolver _resolver;
    static constexpr auto HOST = "127.0.0.1";
    static constexpr auto PORT = 8080;
    std::array<char, 1024> _buffer;

    void startReading();
    void handleRead(const boost::system::error_code& error, std::size_t bytes_transferred);

public:
    Client(boost::asio::io_service& io_service);
    void connect();
    void sendMessage(const std::string& message);


};


