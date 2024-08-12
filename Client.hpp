#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <array>
#include <thread>
#include "Message.hpp"

class Client
{
private:
    static constexpr auto HOST = "127.0.0.1";
    static constexpr auto PORT = 8080;
    static constexpr auto BUFFER_SIZE = 1024;
    static constexpr auto MAX_NAME_SIZE = 30;

    boost::asio::ip::tcp::socket _socket;
    boost::asio::ip::tcp::resolver _resolver;
    std::array<char, BUFFER_SIZE> _buffer;
    std::string _username;

    void startReading();
    void handleRead(const boost::system::error_code& error, std::size_t bytes_transferred);
    void connect();
    void sendMessage(const std::string& message);
    void runIOservice(boost::asio::io_service& io_service);

public:
    Client(boost::asio::io_service& io_service);
    void openClient(boost::asio::io_service& io_service);


};


