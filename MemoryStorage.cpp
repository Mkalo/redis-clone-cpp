#include "MemoryStorage.h"

void MemoryStorage::set(const std::string& key, const std::string& value) {
  std::lock_guard<std::mutex> lock(data_mutex);
  data[key] = value;
}

std::optional<std::string> MemoryStorage::get(const std::string& key) {
  std::lock_guard<std::mutex> lock(data_mutex);
  auto it = data.find(key);
  if (it == data.end()) {
    return std::nullopt;
  }

  return {it->second};
}

void MemoryStorage::del(const std::string& key) {
  std::lock_guard<std::mutex> lock(data_mutex);
  data.erase(key);
}