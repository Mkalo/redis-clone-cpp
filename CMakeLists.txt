cmake_minimum_required(VERSION 3.12)
project(RedisClone)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)

# Add pthread library
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/vendor/asio)

# Add source files
set(SOURCES
    main.cpp
    Lexer.cpp
    MemoryStorage.cpp
    Parser.cpp
    RedisServer.cpp
    Connection.cpp
    CommandsHandler.cpp
)
    
add_executable(RedisClone ${SOURCES})

# Link pthread library
target_link_libraries(RedisClone PRIVATE Threads::Threads)
