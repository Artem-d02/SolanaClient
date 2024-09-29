cmake_minimum_required(VERSION 2.8)

project(SolanaTask)

include_directories(${CMAKE_CXX_INCLUDE_PATH})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")

add_executable(app)
target_sources(app PRIVATE "main.cpp")
target_include_directories(app PRIVATE "${PROJECT_SOURCE_DIR}")

add_subdirectory("method")
add_subdirectory("client")
install(TARGETS app)