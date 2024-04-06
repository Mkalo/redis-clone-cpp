#pragma once

#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>

struct ValueEntry {
  std::string value;
  std::chrono::time_point<std::chrono::system_clock> expiration;
};

class MemoryStorage {
 public:
  void set(const std::string& key, const std::string& value);
  void set(const std::string& key, const std::string& value, int ttl);
  std::optional<std::string> get(const std::string& key);
  void del(const std::string& key);

 private:
  std::unordered_map<std::string, ValueEntry> data;
  std::mutex data_mutex;
};
