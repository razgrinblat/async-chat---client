#include "Client.hpp"

Client::Client(boost::asio::io_service& io_service)
    : _socket(io_service), _resolver(io_service){
}


void Client::connect() 
{
    auto endpoints = _resolver.resolve(HOST, std::to_string(PORT));
    boost::asio::async_connect(_socket, endpoints, [this](const boost::system::error_code& error, const boost::asio::ip::tcp::endpoint& endpoint) {
        if (!error) {
            std::cout << _username << " connected to : " << endpoint.address() << " ,port: " << endpoint.port() << std::endl;
            startReading();
        }
        else {
            throw std::runtime_error("Error connecting to server: " + error.message());
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
    if (!error && bytes_transferred > 0)
    {
        std::string received_message(_buffer.data(), bytes_transferred);
        std::istringstream istream(received_message);
        Message pdu;
        pdu.load(istream);
        std::cout << "[" << pdu.getName() << "] " << pdu.getMessage() << std::endl;
        startReading();
    }
    else {
       throw std::runtime_error("Error reading from server: " + error.message());
    }
}

void Client::sendMessage(const std::string& message) 
{
    std::ostringstream ostream;
    Message pdu(_username,message);
    pdu.save(ostream);
    auto serialized_meesage = ostream.str();
    auto message_ptr = std::make_shared<std::string>(serialized_meesage);
    boost::asio::async_write(_socket, boost::asio::buffer(*message_ptr), [message_ptr](const boost::system::error_code& error, std::size_t bytes) {
        if (error) 
        {
            throw std::runtime_error("Error sending message: " + error.message());
        }
        });
}

void Client::runIOservice(boost::asio::io_service& io_service)
{
    io_service.run(); // This will block and process asynchronous operations until all work is finished
}

void Client::openClient(boost::asio::io_service& io_service)
{   
    std::cout << "Enter your name:";
    do {
        std::getline(std::cin, _username);
        if (_username.size() > MAX_NAME_SIZE)
        {
            std::cout << "name is to long, enter your name again:";
        }
    } while (_username.size() >= MAX_NAME_SIZE);
    this->connect();
    std::thread io_thread(std::bind(&Client::runIOservice, this, std::ref(io_service)));
    io_thread.detach(); 
    std::string message;
    while (std::getline(std::cin, message)) {
        this->sendMessage(message);
    }

}
