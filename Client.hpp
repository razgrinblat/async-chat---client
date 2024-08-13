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

    /**
    * @brief Establishes a connection to the server.
    * @throws std::runtime_error if there is an error connecting to the server.
     */
    void connect();
    /**
    * @brief Begins asynchronously reading messages from the server.
    * Receives the size of the incoming message, allocates a buffer accordingly, and then reads the actual message content.
    */
    void startReading();
    /**
    * @brief Handles the received message from the server.
    * @param error The error code from the asynchronous read operation.
    * @param bytes_transferred The number of bytes transferred.
    * @param bufferptr A shared pointer to the buffer containing the received message.
    * @throws std::runtime_error if there is an error reading from the server.
    */
    void handleRead(const boost::system::error_code& error, std::size_t bytes_transferred, std::shared_ptr<std::vector<char>> bufferptr);
    /**
    * @brief Sends a message to the server.
    * @param message The message string to send.
    * @throws std::runtime_error if there is an error sending the message.
    */
    void sendMessage(const std::string& message);
    /**
 * @brief Runs the Boost.Asio IO service.
 */
    void runIOservice(boost::asio::io_service& io_service);
    /**
    * @brief Assigns a random color to the user.
    */
    void addUserColor();

public:
    Client(boost::asio::io_service& io_service);
    /**
 * @brief Starts the client, connects to the server, and manages user input.
 * @param io_service The Boost.Asio IO service used for asynchronous operations.
 */
    void openClient(boost::asio::io_service& io_service);


};


