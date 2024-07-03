cmake_minimum_required(VERSION 3.10)
project(Catan)

set(CMAKE_CXX_STANDARD 11)

add_executable(Catan main.cpp board.cpp player.cpp game.cpp)
