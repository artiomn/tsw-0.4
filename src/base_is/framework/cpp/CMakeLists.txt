cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

project(base_is)

option(TSW_BUILD_TESTS ON)

add_definitions(-DRAPIDJSON_HAS_STDSTRING)

set (THIRD_PARTY_DIR ../../../third_party)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")

execute_process(COMMAND ln -fs ${THIRD_PARTY_DIR}/args WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
execute_process(COMMAND ln -fs ${THIRD_PARTY_DIR}/asio WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
execute_process(COMMAND ln -fs ${THIRD_PARTY_DIR}/baical_p7 WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
execute_process(COMMAND ln -fs ${THIRD_PARTY_DIR}/ejdb WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
execute_process(COMMAND ln -fs ${THIRD_PARTY_DIR}/rapidjson WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
execute_process(COMMAND ln -fs ${THIRD_PARTY_DIR}/libzmq WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_CXX_FLAGS "-g -Wall -pthread")

find_package(Intl REQUIRED)
#find_package(ZMQ REQUIRED)

#file(STRINGS ejdb/Changelog first_string LIMIT_COUNT 1)
#string(REGEX REPLACE "^.*\\(([^\\)]*)\\).*$" "\\1" ejdb_ver_string "${first_string}")
#set(ejdb_VERSION "${ejdb_ver_string}" CACHE INTERNAL "ejdb_VERSION")
#string(REPLACE "." ";" ejdb_ver_string ${ejdb_ver_string})

#list(GET ejdb_ver_string 0 ejdb_VERSION_MAJOR)
#set(ejdb_VERSION_MAJOR "${ejdb_VERSION_MAJOR}" CACHE INTERNAL "ejdb_VERSION_MAJOR")
#list(GET ejdb_ver_string 1 ejdb_VERSION_MINOR)
#set(ejdb_VERSION_MINOR "${ejdb_VERSION_MINOR}" CACHE INTERNAL "ejdb_VERSION_MINOR")
#list(GET ejdb_ver_string 2 ejdb_VERSION_PATCH)
#set(ejdb_VERSION_PATCH "${ejdb_VERSION_PATCH}" CACHE INTERNAL "ejdb_VERSION_PATCH")

set(ARGS_INCLUDE_DIRS args)
set(EJDB_INCLUDE_DIRS ejdb/src/tcutil ejdb/src/tchdb ejdb/src/tcbdb ejdb/src/tctdb ejdb/src/tcfdb ejdb/src/bson ejdb/src/ejdb
    "${CMAKE_BINARY_DIR}/ejdb/src/generated/")
set(RAPID_JSON_INCLUDE_DIRS rapidjson/include)
set(BAICAL_P7_INCLUDE_DIRS baical_p7/Headers)

file(GLOB headers include/tsw/*.h)
file(GLOB src impl/*.cpp impl/*.h impl/**/*.cpp impl/**/*.h)
file(GLOB trash trash/impl/*.cpp trash/include/tsw/*.h)

file(GLOB rj_headers ${RAPID_JSON_INCLUDE_DIRS}/rapidjson/*.h)
list(APPEND headers ${rj_headers})

add_subdirectory(baical_p7)
#add_subdirectory(libzmq)
add_subdirectory(rapidjson)
add_subdirectory(ejdb)

include_directories(AFTER SYSTEM asio/asio/include)
include_directories(AFTER SYSTEM include ejdb/src ${Intl_INCLUDE_DIRS})
include_directories(AFTER SYSTEM ${ARGS_INCLUDE_DIRS} ${EJDB_INCLUDE_DIRS} ${RAPID_JSON_INCLUDE_DIRS} ${BAICAL_P7_INCLUDE_DIRS})
add_library(${PROJECT_NAME} SHARED ${headers} ${src})

target_link_libraries(${PROJECT_NAME} ${Intl_LIBRARIES} P7_static ejdb pthread zmq stdc++fs)

if (TSW_BUILD_TESTS)
    add_subdirectory(tests)
endif()
