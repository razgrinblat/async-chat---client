#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <array>
#include <thread>
#include <unordered_map>
#include <ctime>
#include "Message.hpp"

class Client
{
private:
    static constexpr auto HOST = "127.0.0.1";
    static constexpr auto PORT = 8080;
    static constexpr auto MAX_NAME_SIZE = 30;
    static constexpr auto RESET_COLOR = "\033[0m";

    boost::asio::ip::tcp::socket _socket;
    boost::asio::ip::tcp::resolver _resolver;
    std::string _username;
    std::string _color;

    void startReading();
    void handleRead(const boost::system::error_code& error, std::size_t bytes_transferred, std::shared_ptr<std::vector<char>> bufferptr);
    void connect();
    void sendMessage(const std::string& message);
    void runIOservice(boost::asio::io_service& io_service);
    void addUserColor();

public:
    Client(boost::asio::io_service& io_service);
    void openClient(boost::asio::io_service& io_service);


};


