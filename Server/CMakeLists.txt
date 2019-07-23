cmake_minimum_required(VERSION 3.1)

find_library(PTHREAD_LIBRARY pthread)

include_directories(include)

add_executable(TestServer src/MainServer.cpp src/TestServer.cpp)

target_link_libraries(TestServer ${PTHREAD_LIBRARY} ${PCRE_LIBRARY})
