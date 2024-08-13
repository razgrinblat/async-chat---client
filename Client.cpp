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
    auto message_size_ptr = std::make_shared<uint32_t>(); 
    _socket.async_receive(boost::asio::buffer(message_size_ptr.get(), sizeof(*message_size_ptr)), [this, message_size_ptr](const boost::system::error_code& error, std::size_t bytes)
        {
            if (!error)
            {

                auto bufferptr = std::make_shared<std::vector<char>>(*message_size_ptr);
                _socket.async_receive(boost::asio::buffer(*bufferptr), [this, bufferptr](const boost::system::error_code& error, std::size_t bytes_transferred)
                    {
                        handleRead(error, bytes_transferred, bufferptr);
                    });

            }
        });
    
    
}

void Client::handleRead(const boost::system::error_code& error, std::size_t bytes_transferred, std::shared_ptr<std::vector<char>> bufferptr) 
{
    if (!error && bytes_transferred > 0)
    {
        std::string received_message(bufferptr->begin(), bufferptr->end());
        std::istringstream istream(received_message);
        Message pdu;
        pdu.load(istream);
        std::cout << pdu.getColor() << "[" << pdu.getName() << "] " << pdu.getMessage() << RESET_COLOR << std::endl;
        startReading();
    }
    else {
       throw std::runtime_error("Error reading from server: " + error.message());
    }
}

void Client::sendMessage(const std::string& message) 
{
    std::ostringstream ostream;
    Message pdu(_color,_username, message);
    pdu.save(ostream);
    auto serialized_meesage = ostream.str();
    uint32_t message_size = serialized_meesage.size(); //send the message size
    boost::asio::async_write(_socket, boost::asio::buffer(&message_size, sizeof(message_size)), [this,serialized_meesage](const boost::system::error_code& error, std::size_t bytes) {
        if (!error)
        {
            auto message_ptr = std::make_shared<std::string>(serialized_meesage);
            boost::asio::async_write(_socket, boost::asio::buffer(*message_ptr), [message_ptr](const boost::system::error_code& error, std::size_t bytes) {
                if (error)
                {
                    throw std::runtime_error("Error sending message: " + error.message());
                }
                }); 
        }
        });
}

void Client::runIOservice(boost::asio::io_service& io_service)
{
    io_service.run(); // This will block and process asynchronous operations until all work is finished
}

void Client::addUserColor()
{
    constexpr auto RED = "\033[31m";
    constexpr auto GREEN = "\033[32m";
    constexpr auto YELLOW = "\033[33m";
    constexpr auto BLUE = "\033[34m";
    constexpr auto MAGENTA = "\033[35m";
    constexpr auto CYAN = "\033[36m";
    constexpr auto BRIGHT_RED = "\033[91m";

    std::unordered_map<int, std::string> clientColors = {
        {1, RED},
        {2, GREEN},
        {3, YELLOW},
        {4, BLUE},
        {5, MAGENTA},
        {6, CYAN},
        {7, BRIGHT_RED}
    };
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Generate a random number between 1 and 7
    int random_number = std::rand() % 7 + 1;

    _color = clientColors[random_number];
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
    addUserColor();
    this->connect();
    std::thread io_thread(std::bind(&Client::runIOservice, this, std::ref(io_service)));
    io_thread.detach(); 
    std::string message;
    while (std::getline(std::cin, message)) {
        this->sendMessage(message);
    }

}
