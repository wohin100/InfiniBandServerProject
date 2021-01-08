cmake_minimum_required(VERSION 3.11)
project(InfiniBandServerProject)

set(CMAKE_CXX_STANDARD 17)

include(ExternalProject)

add_executable(InfiniBandServer main.cpp InfinibandInterface.cpp InfinibandInterface.h Server.cpp Server.h)

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

target_link_libraries(InfiniBandServer PRIVATE InfluxDB)
add_dependencies(InfiniBandServer influx)