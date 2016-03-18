cmake_minimum_required(VERSION 2.8.11)

set(lzwtree_SRCS lzwtree.cpp)

add_executable(lzwtree main.cpp ${lzwtree_SRCS})

target_link_libraries(lzwtree ${Boost_LIBRARIES})