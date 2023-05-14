cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 17)

project(base_is_test)

set(CMAKE_CXX_FLAGS "-g -Wall")

execute_process(COMMAND ln -fs ../../../../third_party/googletest WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
add_subdirectory(googletest)
enable_testing()

include_directories(AFTER SYSTEM googletest/googletest/include googletest/googlemock/include ../include ../)

#find_package(GTest REQUIRED PATHS ../../../third_party/googletest)

file(GLOB src *)
#include_directories(AFTER SYSTEM ../include ../../../../third_party/googletest/googletest/include)

add_executable(tester ${src})
target_link_libraries(tester ${GOOGLE_TEST} gtest gtest_main base_is zmq pthread)

add_test(NAME base_is_test COMMAND tester)
