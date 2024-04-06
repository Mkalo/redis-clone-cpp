#include "MemoryStorage.h"

void MemoryStorage::set(const std::string& key, const std::string& value) {
  std::lock_guard<std::mutex> lock(data_mutex);
  data[key] = {value, std::chrono::system_clock::time_point::max()};
}

void MemoryStorage::set(const std::string& key, const std::string& value, int ttl) {
  std::lock_guard<std::mutex> lock(data_mutex);
  data[key] = {value, std::chrono::system_clock::now() + std::chrono::milliseconds(ttl)};
}

std::optional<std::string> MemoryStorage::get(const std::string& key) {
  std::lock_guard<std::mutex> lock(data_mutex);
  auto it = data.find(key);

  if (it == data.end()) {
    return std::nullopt;
  }

  // Check if the key has expired
  if (it->second.expiration < std::chrono::system_clock::now()) {
    data.erase(it);
    return std::nullopt;
  }

  return { it->second.value };
}

void MemoryStorage::del(const std::string& key) {
  std::lock_guard<std::mutex> lock(data_mutex);
  data.erase(key);
}
