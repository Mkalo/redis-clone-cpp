#pragma once

#include "Lexer.h"

enum class RespType { SimpleString, Error, Integer, BulkString, Array };

class RespObject {
 public:
  virtual ~RespObject() {}
  virtual RespType getType() const = 0;
  virtual std::string encode() const = 0;
};

class SimpleString : public RespObject {
 public:
  SimpleString(const std::string& value) : value(value) {}
  SimpleString(std::string&& value) : value(std::move(value)) {}
  RespType getType() const override { return RespType::SimpleString; }
  const std::string& getValue() const { return value; }
  std::string encode() const { return "+" + value + "\r\n"; }

 private:
  std::string value;
};

class Error : public RespObject {
 public:
  Error(const std::string& value) : value(value) {}
  Error(std::string&& value) : value(std::move(value)) {}
  RespType getType() const override { return RespType::Error; }
  const std::string& getValue() const { return value; }
  std::string encode() const { return "-" + value + "\r\n"; }

 private:
  std::string value;
};

class Integer : public RespObject {
 public:
  Integer(int value) : value(value) {}
  RespType getType() const override { return RespType::Integer; }
  int getValue() const { return value; }
  std::string encode() const { return ":" + std::to_string(value) + "\r\n"; }

 private:
  int value;
};

class BulkString : public RespObject {
 public:
  BulkString(const std::string& value) : value(value) {}
  BulkString(std::string&& value) : value(std::move(value)) {}
  BulkString() : isNull(true) {}
  RespType getType() const override { return RespType::BulkString; }
  const std::string& getValue() const { return value; }
  std::string encode() const {
    if (isNull) {
      return "$-1\r\n";
    }
    return "$" + std::to_string(value.size()) + "\r\n" + value + "\r\n";
  }

 private:
  std::string value;
  bool isNull = false;
};

class Array : public RespObject {
 public:
  Array(const std::vector<std::shared_ptr<RespObject>>& value) : value(value) {}
  Array(std::vector<std::shared_ptr<RespObject>>&& value)
      : value(std::move(value)) {}
  RespType getType() const override { return RespType::Array; }
  const std::vector<std::shared_ptr<RespObject>>& getValue() const {
    return value;
  }
  std::string encode() const {
    std::string result = "*" + std::to_string(value.size()) + "\r\n";
    for (const auto& object : value) {
      result += object->encode();
    }
    return result;
  }

 private:
  std::vector<std::shared_ptr<RespObject>> value;
};

class Parser {
 public:
  std::vector<std::shared_ptr<RespObject>> parse(
      const std::vector<Lexer::Token>& tokens);

 private:
  std::shared_ptr<RespObject> parseToken(
      const std::vector<Lexer::Token>& tokens, size_t& index);
  std::shared_ptr<RespObject> parseSimpleString(
      const std::vector<Lexer::Token>& tokens, size_t& index);
  std::shared_ptr<RespObject> parseError(
      const std::vector<Lexer::Token>& tokens, size_t& index);
  std::shared_ptr<RespObject> parseInteger(
      const std::vector<Lexer::Token>& tokens, size_t& index);
  std::shared_ptr<RespObject> parseBulkString(
      const std::vector<Lexer::Token>& tokens, size_t& index);
  std::shared_ptr<RespObject> parseArray(
      const std::vector<Lexer::Token>& tokens, size_t& index);
};
