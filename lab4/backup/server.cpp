#include <iostream>
#include <boost/asio.hpp>
#include <list>

#include "cabin.h"

using boost::asio::ip::tcp;

class ElevatorSession : public std::enable_shared_from_this<ElevatorSession> {
public:
    ElevatorSession(tcp::socket socket, std::list<std::shared_ptr<ElevatorSession>>& clients, std::shared_ptr<Cabin> cabin)
        : socket_(std::move(socket)), clients_(clients), m_cabin(cabin) {}

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

                    if (message == "o")
                    {
                        m_cabin->open_doors();
                    }
                    else if (message == "c")
                    {
                        m_cabin->close_doors();
                    }
                    else if (message == "u")
                    {
                        m_cabin->move_up();
                    }
                    else if (message == "d")
                    {
                        m_cabin->move_down();
                    }
                    else if (message == "s")
                    {
                        m_cabin->stop();
                    }
                    else if (message == "p")
                    {
                        m_cabin->print_state();
                    }
                    else if (message == "dp")
                    {
                        m_cabin->m_doors.print_state();
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
    std::shared_ptr<Cabin> m_cabin;
};

class ElevatorServer {
public:
    ElevatorServer(boost::asio::io_context& ioContext, short port)
        : acceptor_(ioContext, tcp::endpoint(tcp::v4(), port))
    {
        m_cabin = std::make_shared<Cabin>(ioContext);

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

                    std::shared_ptr<ElevatorSession> session = std::make_shared<ElevatorSession>(std::move(socket), clients_, m_cabin);
                    session->start();
                }

                startAccept();
            });
    }

    tcp::acceptor acceptor_;
    std::list<std::shared_ptr<ElevatorSession>> clients_;
    std::shared_ptr<Cabin> m_cabin;
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
