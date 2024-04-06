#include "Connection.h"

void Connection::async_read() {
  socket_.async_read_some(
      asio::buffer(*buffer),
      [self = shared_from_this()](asio::error_code ec, std::size_t length) {
        if (!ec) {
          self->handle_read(length);
        }
      });
}

void Connection::handle_read(std::size_t length) {
  auto tokens = lexer.tokenize(std::string_view(buffer->data(), length));
  auto objects = parser.parse(tokens);
  auto res = commands_handler.handle(objects);

  std::string response = res->encode();

  async_write(response);
}

void Connection::async_write(const std::string& response) {
  auto self = shared_from_this();
  asio::async_write(
      socket_, asio::buffer(response),
      [self](asio::error_code ec, std::size_t /*bytes_transferred*/) {
        if (!ec) {
          self->async_read();  // Continue reading from the socket
        }
      });
}
