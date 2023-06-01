#include <iostream>
#include <boost/asio.hpp>

void handle_request(const boost::system::error_code& error)
{
    if (!error)
    {
        std::cout << "Request handled successfully" << std::endl;
    }
    else
    {
        std::cerr << "Error handling request: " << error.message() << std::endl;
    }
}

int main()
{
    boost::asio::io_context io_context;

    // Create a TCP acceptor
    boost::asio::ip::tcp::acceptor acceptor(io_context,
        boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 12345));

    // Create a TCP socket
    boost::asio::ip::tcp::socket socket(io_context);

    // Perform an asynchronous accept operation
    acceptor.async_accept(socket, [&](const boost::system::error_code& error) {
        if (!error)
        {
            std::cout << "Accepted incoming connection" << std::endl;

            // Handle the request asynchronously
            handle_request(error);
        }
        else
        {
            std::cerr << "Error accepting connection: " << error.message() << std::endl;
        }
    });

    // Start the IO context
    io_context.run();

    return 0;
}
