cmake_minimum_required(VERSION 2.8.11)

project(lzwtree)

find_package(Boost COMPONENTS program_options REQUIRED)

set(CMAKE_CXX_FLAGS "-std=c++14 -o3")

add_subdirectory(src)
