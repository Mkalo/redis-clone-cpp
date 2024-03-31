cmake_minimum_required(VERSION 3.12)
project(RedisClone)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)

# Find Boost
find_package(Boost 1.74 REQUIRED COMPONENTS system)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    
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
    target_link_libraries(RedisClone PRIVATE ${Boost_LIBRARIES})
endif()
