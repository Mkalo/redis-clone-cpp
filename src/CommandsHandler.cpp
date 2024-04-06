#include "CommandsHandler.h"

MemoryStorage g_memoryStorage;

std::unique_ptr<RespObject> ping_command(
    const std::vector<std::shared_ptr<RespObject>>& objects) {
  if (objects.size() == 1) {
    return std::make_unique<SimpleString>("PONG");
  } else if (objects.size() == 2) {
    auto message = std::static_pointer_cast<BulkString>(objects[1]);
    return std::make_unique<BulkString>(message->getValue());
  } else {
    return std::make_unique<Error>(
        "ERR wrong number of arguments for 'ping' command");
  }
}

std::unique_ptr<RespObject> echo_command(
    const std::vector<std::shared_ptr<RespObject>>& objects) {
  if (objects.size() != 2) {
    return std::make_unique<Error>(
        "ERR wrong number of arguments for 'echo' command");
  }

  auto message = std::static_pointer_cast<BulkString>(objects[1]);

  return std::make_unique<BulkString>(message->getValue());
}

std::unique_ptr<RespObject> set_command(
    const std::vector<std::shared_ptr<RespObject>>& objects) {
  if (objects.size() < 3) {
    return std::make_unique<Error>(
        "ERR wrong number of arguments for 'set' command");
  }

  auto key =
      std::string(std::static_pointer_cast<BulkString>(objects[1])->getValue());
  auto value =
      std::string(std::static_pointer_cast<BulkString>(objects[2])->getValue());

  g_memoryStorage.set(key, value);

  return std::make_unique<SimpleString>("OK");
}

std::unique_ptr<RespObject> get_command(
    const std::vector<std::shared_ptr<RespObject>>& objects) {
  if (objects.size() != 2) {
    return std::make_unique<Error>(
        "ERR wrong number of arguments for 'get' command");
  }

  auto key =
      std::string(std::static_pointer_cast<BulkString>(objects[1])->getValue());

  auto value = g_memoryStorage.get(key);

  if (!value.has_value()) {
    return std::make_unique<BulkString>();
  }

  return std::make_unique<BulkString>(value.value());
}

std::unique_ptr<RespObject> CommandsHandler::handle(
    const std::vector<std::shared_ptr<RespObject>>& objects) {
  if (objects.empty()) {
    return std::make_unique<Error>("Empty command");
  }

  auto command = objects[0];
  if (command->getType() != RespType::Array) {
    return std::make_unique<Error>("Invalid command");
  }

  auto command_array = std::static_pointer_cast<Array>(command)->getValue();

  if (command_array.empty()) {
    return std::make_unique<Error>("Empty command");
  }

  // All elements in the array should be BulkStrings
  for (const auto& object : command_array) {
    if (object->getType() != RespType::BulkString) {
      return std::make_unique<Error>("Invalid command");
    }
  }

  auto command_name =
      std::static_pointer_cast<BulkString>(command_array[0])->getValue();

  // Uppercase the command name
  std::transform(command_name.begin(), command_name.end(), command_name.begin(),
                 ::toupper);
  auto it = commands.find(command_name);

  if (it == commands.end()) {
    return std::make_unique<Error>("Unknown command");
  }

  return it->second(command_array);
}