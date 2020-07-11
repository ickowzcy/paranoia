cmake_minimum_required(VERSION 3.16)
project(paranoia)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-g -pthread")

set(INCLUDE ./include)
set(SOURCES ./src/*.cpp)

file(GLOB_RECURSE SOURCES "${SOURCES}")
include_directories(${INCLUDE})

include(FetchContent)

# nlohmann/json
FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.7.3)
FetchContent_GetProperties(json)
if(NOT json_POPULATED)
    FetchContent_Populate(json)
endif()
add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
include_directories(${json_SOURCE_DIR}/include)

# catch2 (tests)
FetchContent_Declare(
        catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
)
FetchContent_GetProperties(catch2)
if(NOT catch2_POPULATED)
    FetchContent_Populate(catch2)
endif()
add_subdirectory(${catch2_SOURCE_DIR} ${catch2_BINARY_DIR} EXCLUDE_FROM_ALL)
add_executable(
        tests tests/tests.cpp
        src/kv_serializer.cpp tests/kv_serializer_test.cpp
        include/msg_queue.h tests/msg_queue_test.cpp
        include/util/concat.h tests/util_test.cpp
)
target_link_libraries(tests Catch2::Catch2)

add_executable(paranoia ${SOURCES})