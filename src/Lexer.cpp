#include "Lexer.h"

std::vector<Lexer::Token> Lexer::tokenize(std::string_view message) {
  std::vector<Token> tokens;

  size_t index = 0;
  while (index < message.size()) {
    // Check for CRLF sequence
    if (message[index] == '\r' && index + 1 < message.size() &&
        message[index + 1] == '\n') {
      index += 2;  // skip \r\n
      continue;
    }

    RespTokenType type;
    std::optional<std::string_view> value;

    switch (message[index]) {
      case '+':
        type = RespTokenType::SimpleString;
        break;
      case '-':
        type = RespTokenType::Error;
        break;
      case ':':
        type = RespTokenType::Integer;
        break;
      case '$':
        type = RespTokenType::BulkString;
        break;
      case '*':
        type = RespTokenType::Array;
        break;
      default: {
        // Find end of literal
        size_t end_index = message.find("\r\n", index);
        if (end_index == std::string::npos) {
          // If not found, take the rest of the string
          value = message.substr(index);
          index = message.size();  // exit loop
        } else {
          value = message.substr(index, end_index - index);
          index = end_index - 1;  // Move index to end of literal
        }
        type = RespTokenType::Literal;
      }
    }

    tokens.push_back({type, value});
    ++index;  // Move to next character
  }

  return tokens;
}
