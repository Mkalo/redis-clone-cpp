#pragma once

#include <algorithm>
#include <functional>
#include <unordered_map>

#include "MemoryStorage.h"
#include "Parser.h"

std::unique_ptr<RespObject> ping_command(
    const std::vector<std::shared_ptr<RespObject>>& objects);
std::unique_ptr<RespObject> echo_command(
    const std::vector<std::shared_ptr<RespObject>>& objects);
std::unique_ptr<RespObject> set_command(
    const std::vector<std::shared_ptr<RespObject>>& objects);
std::unique_ptr<RespObject> get_command(
    const std::vector<std::shared_ptr<RespObject>>& objects);

// Map command names to their corresponding functions
const std::unordered_map<std::string,
                         std::function<std::unique_ptr<RespObject>(
                             const std::vector<std::shared_ptr<RespObject>>&)>>
    commands = {
        {"PING", ping_command},
        {"ECHO", echo_command},
        {"SET", set_command},
        {"GET", get_command},
};

class CommandsHandler {
 public:
  std::unique_ptr<RespObject> handle(
      const std::vector<std::shared_ptr<RespObject>>& objects);
};
