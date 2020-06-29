cmake_minimum_required(VERSION 3.16)
project(paranoia)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-g -pthread")

set(INCLUDE ./include)
set(SOURCES ./src/*.cpp)

file(GLOB SOURCES "${SOURCES}")

include(FetchContent)

FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.7.3)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
    FetchContent_Populate(json)
endif()
add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)

include_directories(${INCLUDE} ${json_SOURCE_DIR}/include)

add_executable(paranoia ${SOURCES})