#include <iostream>
#include <boost/asio.hpp>
#include <list>
#include <regex>

#include "controller.h"

namespace Color {
    const std::string Yellow = "\033[33m";
    const std::string Reset = "\033[0m";
}

using boost::asio::ip::tcp;

class ElevatorSession : public std::enable_shared_from_this<ElevatorSession> {
public:
    ElevatorSession(tcp::socket socket, std::list<std::shared_ptr<ElevatorSession>>& clients, std::shared_ptr<Controller> controller)
        : socket_(std::move(socket)), clients_(clients), m_controller(controller) {}

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

                    std::regex cabin_button_pattern("c(\\d+)");
                    std::regex floor_button_pattern("f(\\d+)");
                    std::smatch matches;

                    int button = 0;

                    if (std::regex_search(message, matches, cabin_button_pattern))
                    {
                        button = std::stoi(matches.str(1));

                        m_controller->activate_cabin_button(button);
                    }
                    else if (std::regex_search(message, matches, floor_button_pattern))
                    {
                        button = std::stoi(matches.str(1));

                        m_controller->activate_floor_button(button);
                    }
                    else if (message == "p")
                    {
                        std::cout << Color::Yellow << std::endl;
                        m_controller->print_state();
                        std::cout << Color::Reset << std::endl;
                    }
                    else if (message == "r" || message == "q")
                    {
                        m_controller->cancel_cabin_buttons();
                    }
                    else if (message == "c")
                    {
                        m_controller->m_cabin.close_doors();
                    }
                    else if (message == "o")
                    {
                        m_controller->m_cabin.open_doors();
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
    std::shared_ptr<Controller> m_controller;
};

class ElevatorServer {
public:
    ElevatorServer(boost::asio::io_context& ioContext, short port)
        : acceptor_(ioContext, tcp::endpoint(tcp::v4(), port))
    {
        m_controller = std::make_shared<Controller>(ioContext);

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

                    std::shared_ptr<ElevatorSession> session = std::make_shared<ElevatorSession>(std::move(socket), clients_, m_controller);
                    session->start();
                }

                startAccept();
            });
    }

    tcp::acceptor acceptor_;
    std::list<std::shared_ptr<ElevatorSession>> clients_;
    std::shared_ptr<Controller> m_controller;
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
