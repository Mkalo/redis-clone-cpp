#pragma once

#include <array>
#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <mutex>

#include "CommandsHandler.h"
#include "Lexer.h"
#include "Parser.h"

class Connection : public std::enable_shared_from_this<Connection> {
 public:
  Connection(boost::asio::io_context& io_context)
      : socket_(io_context),
        buffer(std::make_shared<std::array<char, 8192>>()) {}

  void start() { async_read(); }

  boost::asio::ip::tcp::socket& socket() { return socket_; }

 private:
  void async_read();
  void handle_read(std::size_t length);
  void async_write(const std::string& response);

  boost::asio::ip::tcp::socket socket_;
  std::shared_ptr<std::array<char, 8192>> buffer;
  Lexer lexer;
  Parser parser;
  CommandsHandler commands_handler;
};
