cmake_minimum_required(VERSION 3.1)

find_library(PTHREAD_LIBRARY pthread)

include_directories(include)

add_executable(TestClient src/MainClient.cpp src/TestClient.cpp)

target_link_libraries(TestClient ${PTHREAD_LIBRARY} ${PCRE_LIBRARY})
