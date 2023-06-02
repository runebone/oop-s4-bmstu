#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class ChatClient {
public:
    ChatClient(boost::asio::io_context& ioContext, const tcp::resolver::results_type& endpoints)
        : socket_(ioContext)
    {
        boost::asio::async_connect(socket_, endpoints,
            [this](boost::system::error_code ec, tcp::endpoint)
            {
                if (!ec)
                {
                    std::cout << "Connected to server" << std::endl;

                    std::cout << "Enter your name: ";
                    std::getline(std::cin, name_);

                    startRead();
                    startWrite();
                }
            });

        readThread_ = std::thread([this, &ioContext]() { ioContext.run(); });
    }

    void startWrite()
    {
        /* std::cout << "> "; */
        std::string message;
        std::getline(std::cin, message);

        {
            message = decorateMessage(message);

            boost::asio::async_write(socket_, boost::asio::buffer(message),
                [this](boost::system::error_code ec, std::size_t /*bytesTransferred*/)
                {
                    if (!ec)
                    {
                        startWrite();
                    }
                });
        }
    }

/* private: */
    std::string decorateMessage(std::string message)
    {
        return "[" + name_ + "]: " + message + "\n";
    }

    void startRead()
    {
        boost::asio::async_read_until(socket_, inputBuffer_, '\n',
            [this](boost::system::error_code ec, std::size_t /*bytesTransferred*/)
            {
                if (!ec)
                {
                    std::istream stream(&inputBuffer_);
                    std::string message;
                    std::getline(stream, message);

                    /* std::cout << "\r"; */
                    std::cout << message << std::endl;

                    startRead();
                }
            });
    }

    std::string name_;
    tcp::socket socket_;
    boost::asio::streambuf inputBuffer_;
    std::thread readThread_;
};

int main()
{
    try
    {
        boost::asio::io_context ioContext;

        tcp::resolver resolver(ioContext);
        tcp::resolver::results_type endpoints = resolver.resolve("localhost", "12345");

        ChatClient client(ioContext, endpoints);

        ioContext.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
