#pragma once

#include <iostream>
#include <optional>
#include <string>
#include <vector>

enum class RespTokenType {
  SimpleString,
  Error,
  Integer,
  BulkString,
  Array,
  Literal,
};

class Lexer {
 public:
  struct Token {
    RespTokenType type;
    std::optional<std::string_view> value;
  };

  std::vector<Token> tokenize(std::string_view message);
};
