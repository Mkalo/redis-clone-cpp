#include "Parser.h"

std::vector<std::shared_ptr<RespObject>> Parser::parse(
    const std::vector<Lexer::Token>& tokens) {
  std::vector<std::shared_ptr<RespObject>> objects;
  objects.reserve(tokens.size());  // Reserve space for efficiency

  size_t index = 0;

  while (index < tokens.size()) {
    objects.push_back(parseToken(tokens, index));
  }

  return objects;
}

std::shared_ptr<RespObject> Parser::parseToken(
    const std::vector<Lexer::Token>& tokens, size_t& index) {
  const auto& token = tokens[index];

  switch (token.type) {
    case RespTokenType::SimpleString:
      return parseSimpleString(tokens, index);
    case RespTokenType::Error:
      return parseError(tokens, index);
    case RespTokenType::Integer:
      return parseInteger(tokens, index);
    case RespTokenType::BulkString:
      return parseBulkString(tokens, index);
    case RespTokenType::Array:
      return parseArray(tokens, index);
    default:
      throw std::runtime_error("Invalid token type");
  }
}

std::shared_ptr<RespObject> Parser::parseSimpleString(
    const std::vector<Lexer::Token>& tokens, size_t& index) {
  // Next token should be a literal
  if (index + 1 >= tokens.size() || !tokens[index + 1].value.has_value()) {
    throw std::runtime_error("Invalid SimpleString token");
  }

  const auto& literal = tokens[index + 1].value.value();

  index += 2;  // Move to next token

  return std::make_shared<SimpleString>(std::string(literal));
}

std::shared_ptr<RespObject> Parser::parseError(
    const std::vector<Lexer::Token>& tokens, size_t& index) {
  // Next token should be a literal
  if (index + 1 >= tokens.size() || !tokens[index + 1].value.has_value()) {
    throw std::runtime_error("Invalid Error token");
  }

  const auto& literal = tokens[index + 1].value.value();

  index += 2;  // Move to next token

  return std::make_shared<Error>(std::string(literal));
}

std::shared_ptr<RespObject> Parser::parseInteger(
    const std::vector<Lexer::Token>& tokens, size_t& index) {
  // Next token should be a literal
  if (index + 1 >= tokens.size() || !tokens[index + 1].value.has_value()) {
    throw std::runtime_error("Invalid Integer token");
  }

  const auto& literal = tokens[index + 1].value.value();

  index += 2;  // Move to next token

  return std::make_shared<Integer>(std::stoi(std::string(literal)));
}

std::shared_ptr<RespObject> Parser::parseBulkString(
    const std::vector<Lexer::Token>& tokens, size_t& index) {
  // Next token should be a literal
  if (index + 1 >= tokens.size() || !tokens[index + 1].value.has_value()) {
    throw std::runtime_error("Invalid BulkString token");
  }

  const auto& size_literal = tokens[index + 1].value.value();

  auto size = std::stoi(std::string(size_literal));

  // Next token should be a literal
  if (index + 2 >= tokens.size() || !tokens[index + 2].value.has_value()) {
    throw std::runtime_error("Invalid BulkString token");
  }

  const auto& literal = tokens[index + 2].value.value();

  // Check if the size of the literal matches the size
  if (literal.size() != size) {
    throw std::runtime_error("Invalid BulkString size");
  }

  index += 3;  // Move to next token

  return std::make_shared<BulkString>(std::string(literal));
}

std::shared_ptr<RespObject> Parser::parseArray(
    const std::vector<Lexer::Token>& tokens, size_t& index) {
  // Next token should be a literal
  if (index + 1 >= tokens.size() || !tokens[index + 1].value.has_value()) {
    throw std::runtime_error("Invalid Array token");
  }

  const auto& size_literal = tokens[index + 1].value.value();

  auto size = std::stoi(std::string(size_literal));

  std::vector<std::shared_ptr<RespObject>> value(size);

  index += 2;  // Move to next token

  for (int i = 0; i < size; ++i) {
    if (index >= tokens.size()) {
      throw std::runtime_error("Invalid Array size");
    }
    value[i] = parseToken(tokens, index);
  }

  return std::make_shared<Array>(std::move(value));
}
