#include "RedisServer.h"

RedisServer::RedisServer() : acceptor(io_context) {}

void RedisServer::start() {
  try {
    acceptor.open(tcp::v4());
    acceptor.set_option(tcp::acceptor::reuse_address(true));
    acceptor.bind(tcp::endpoint(tcp::v4(), 6379));  // Default Redis port
    acceptor.listen();

    std::cout << "Server listening on port 6379..." << std::endl;

    accept_connections();

    int buffer_size = 8192;
    acceptor.set_option(asio::socket_base::receive_buffer_size(buffer_size));

    io_context.run();
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
}

void RedisServer::accept_connections() {
  auto new_connection = std::make_shared<Connection>(io_context);

  acceptor.async_accept(
      new_connection->socket(), [this, new_connection](asio::error_code ec) {
        if (!ec) {
          new_connection->socket().set_option(asio::ip::tcp::no_delay(true));
          new_connection->start();
        }
        accept_connections();  // Continue accepting new connections
      });
}
