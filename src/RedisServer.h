#pragma once
#include <iostream>
#include <mutex>
#include <vector>
#include <asio.hpp>

#include "CommandsHandler.h"
#include "Connection.h"
#include "Lexer.h"
#include "Parser.h"

using tcp = asio::ip::tcp;

class RedisServer {
 public:
  RedisServer();
  void start();

 private:
  void accept_connections();

  asio::io_context io_context{8};
  tcp::acceptor acceptor;
};