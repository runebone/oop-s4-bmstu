#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace Color {
    const std::string Blue = "\033[34m";
    const std::string Reset = "\033[0m";
}

class ElevatorClient {
public:
    ElevatorClient(boost::asio::io_context& ioContext, const tcp::resolver::results_type& endpoints)
        : socket_(ioContext)
    {
        boost::asio::async_connect(socket_, endpoints,
            [this](boost::system::error_code ec, tcp::endpoint)
            {
                if (!ec)
                {
                    std::cout << "Connected to Elevator server." << std::endl;

                    std::cout << std::endl << "Управление лифтом:" << std::endl;
                    std::cout << Color::Blue << "f{N}" << Color::Reset << " - Нажать кнопку вызова лифта на N этаже (1 <= N <= 12)" << std::endl;
                    std::cout << Color::Blue << "c{N}" << Color::Reset << " - Нажать кнопку N этажа в кабине лифта (Пример: c4)" << std::endl;
                    std::cout << Color::Blue << " p  " << Color::Reset << " - Вывести текущее состояние лифта" << std::endl;
                    std::cout << Color::Blue << "q/r " << Color::Reset << " - Нажать на кнопку отмены в лифте" << std::endl;
                    std::cout << Color::Blue << " o  " << Color::Reset << " - Нажать на кнопку открытия дверей" << std::endl;
                    std::cout << Color::Blue << " o  " << Color::Reset << " - Нажать на кнопку закрытия дверей" << std::endl;
                    std::cout << std::endl;

                    startRead();
                    startWrite();
                }
            });

        readThread_ = std::thread([this, &ioContext]() { ioContext.run(); });
    }

    void startWrite()
    {
        std::string message;
        std::getline(std::cin, message);
        message += '\n';

        {
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

        ElevatorClient client(ioContext, endpoints);

        ioContext.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
