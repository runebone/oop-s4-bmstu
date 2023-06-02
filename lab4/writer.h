#pragma once

#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Writer
{
public:
    Writer(boost::asio::io_context& ioContext, const tcp::resolver::results_type& endpoints)
        : m_socket(ioContext)
    {
        boost::asio::async_connect(m_socket, endpoints,
            [this](boost::system::error_code ec, tcp::endpoint) {});
    }

    void write(std::string message)
    {
        message += '\n'; // XXX
        boost::asio::async_write(m_socket, boost::asio::buffer(message),
                [this](boost::system::error_code ec,
                    std::size_t /*bytesTransferred*/) {});
    }

private:
    tcp::socket m_socket;
};
