#include <iostream>
#include <boost/asio.hpp>
#include <list>
#include <regex>

#include "elevator.h"

namespace Color {
    const std::string Yellow = "\033[33m";
    const std::string Reset = "\033[0m";
}

using boost::asio::ip::tcp;

class ElevatorSession : public std::enable_shared_from_this<ElevatorSession> {
public:
    ElevatorSession(tcp::socket socket, std::list<std::shared_ptr<ElevatorSession>>& clients, std::shared_ptr<Elevator> elevator)
        : socket_(std::move(socket)), clients_(clients), m_elevator(elevator) {}

    void start()
    {
        clients_.push_back(shared_from_this());

        doRead();
    }

    void broadcast(const std::string& message)
    {
        for (const auto& client: clients_)
        {
            if (client != shared_from_this())
                boost::asio::async_write(client->socket_, boost::asio::buffer(message),
                    [](boost::system::error_code /*ec*/, std::size_t /*bytesTransferred*/) {});
        }
    }

private:
    void doRead()
    {
        auto self(shared_from_this());
        boost::asio::async_read_until(socket_, inputBuffer_, '\n',
            [this, self](boost::system::error_code ec, std::size_t bytesTransferred)
            {
                if (!ec)
                {
                    std::istream stream(&inputBuffer_);
                    std::string message;
                    std::getline(stream, message);

                    /* std::cout << message << std::endl; */

                    std::regex elevator_button_pattern("c(\\d+)");
                    std::regex floor_button_pattern("f(\\d+)");
                    std::smatch matches;

                    int button = 0;

                    if (std::regex_search(message, matches, elevator_button_pattern))
                    {
                        button = std::stoi(matches.str(1));

                        m_elevator->activate_floor_button(button, IN_CABIN);
                    }
                    else if (std::regex_search(message, matches, floor_button_pattern))
                    {
                        button = std::stoi(matches.str(1));

                        m_elevator->activate_floor_button(button, ON_FLOOR);
                    }
                    else if (message == "p")
                    {
                        std::cout << Color::Yellow << std::endl;
                        /* m_elevator->print_state(); */
                        m_elevator->print();
                        std::cout << Color::Reset << std::endl;
                    }
                    else if (message == "c")
                    {
                        m_elevator->activate_cabin_cancel_button();
                    }

                    /* message += '\n'; */
                    /* broadcast(message); */

                    doRead();
                }
                else if (ec == boost::asio::error::eof || ec == boost::asio::error::connection_reset)
                {
                    std::cerr << "Client disconnected." << std::endl;
                    clients_.remove(shared_from_this()); // Remove this session from the list of clients
                }
                else
                {
                    std::cerr << "Error reading from client: " << ec.message() << std::endl;
                    clients_.remove(shared_from_this()); // Remove this session from the list of clients
                }
            });
    }

    tcp::socket socket_;
    boost::asio::streambuf inputBuffer_;
    std::list<std::shared_ptr<ElevatorSession>>& clients_;
    std::shared_ptr<Elevator> m_elevator;
};

class ElevatorServer {
public:
    ElevatorServer(boost::asio::io_context& ioContext, short port)
        : acceptor_(ioContext, tcp::endpoint(tcp::v4(), port))
    {
        m_elevator = std::make_shared<Elevator>(ioContext);

        startAccept();
    }

private:
    void startAccept()
    {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket)
            {
                if (!ec)
                {
                    std::cout << "New client connected." << std::endl;

                    std::shared_ptr<ElevatorSession> session = std::make_shared<ElevatorSession>(std::move(socket), clients_, m_elevator);
                    session->start();
                }

                startAccept();
            });
    }

    tcp::acceptor acceptor_;
    std::list<std::shared_ptr<ElevatorSession>> clients_;
    std::shared_ptr<Elevator> m_elevator;
};

int main()
{
    try
    {
        boost::asio::io_context ioContext;

        ElevatorServer server(ioContext, 12345);

        ioContext.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
