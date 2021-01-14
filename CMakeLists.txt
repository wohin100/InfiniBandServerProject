cmake_minimum_required(VERSION 3.11)
project(InfiniBandServerProject)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

include(ExternalProject)

add_executable(InfiniBandServer main.cpp InfluxDbInterface.cpp InfluxDbInterface.h Server.cpp Server.h MultiThreadServer.cpp MultiThreadServer.h)

include(ExternalProject)

ExternalProject_Add(
        influx
        GIT_REPOSITORY https://github.com/awegrzyn/influxdb-cxx
        GIT_TAG v0.5.1
        CMAKE_ARGS "${CMAKE_ARGS};-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/ext_build;"
)

target_link_directories(InfiniBandServer PUBLIC "${CMAKE_BINARY_DIR}/ext_build/lib/")

ExternalProject_Get_property(influx SOURCE_DIR)
ExternalProject_Get_property(influx BINARY_DIR)

target_include_directories(InfiniBandServer PRIVATE "${SOURCE_DIR}/include")
target_include_directories(InfiniBandServer PRIVATE "${BINARY_DIR}/include")

add_dependencies(InfiniBandServer influx)

include(FetchContent)

FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
        GIT_TAG v3.7.3)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
    FetchContent_Populate(json)
    add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

target_link_libraries(InfiniBandServer nlohmann_json::nlohmann_json InfluxDB)