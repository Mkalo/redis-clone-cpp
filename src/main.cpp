#include "RedisServer.h"

int main(int argc, char **argv) {
  // We should accept --port command line argument
  int PORT = 6379;

  if (argc == 3 && std::string(argv[1]) == "--port") {
    try {
      PORT = std::stoi(argv[2]);
    } catch (const std::exception& e) {
      std::cerr << "Invalid port number" << std::endl;
      return 1;
    }
  }

  RedisServer server(PORT);
  server.start();
}
