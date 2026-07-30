cmake_minimum_required(VERSION 3.15)
project(meteosource_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(CURL REQUIRED)

# jsoncpp discovery.
#
# The sources include <json/json.h>, so the include path handed to the
# compiler must contain the directory that holds "json/json.h":
#   - vcpkg / upstream installs: <prefix>/include/json/json.h and the
#     CMake config's imported target already exports <prefix>/include.
#   - Debian/Ubuntu (libjsoncpp-dev): /usr/include/jsoncpp/json/json.h;
#     both the shipped CMake config and jsoncpp.pc export
#     /usr/include/jsoncpp as the include directory.
# In every case the exported include directory makes <json/json.h> work,
# so we just need to find *some* usable jsoncpp target:
#   1. CMake package config (vcpkg, upstream, and recent distro packages),
#      which may expose JsonCpp::JsonCpp or the legacy jsoncpp_lib target.
#   2. pkg-config fallback for distro packages without a CMake config.
find_package(jsoncpp CONFIG QUIET)
if(TARGET JsonCpp::JsonCpp)
    set(METEOSOURCE_JSONCPP_TARGET JsonCpp::JsonCpp)
elseif(TARGET jsoncpp_lib)
    set(METEOSOURCE_JSONCPP_TARGET jsoncpp_lib)
elseif(TARGET jsoncpp_static)
    set(METEOSOURCE_JSONCPP_TARGET jsoncpp_static)
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(JSONCPP REQUIRED IMPORTED_TARGET jsoncpp)
    set(METEOSOURCE_JSONCPP_TARGET PkgConfig::JSONCPP)
endif()

add_library(meteosource STATIC
    src/AirQuality.cpp
    src/Data.cpp
    src/Forecast.cpp
    src/Meteosource.cpp
    src/RequestHandler.cpp
)
target_include_directories(meteosource PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(meteosource PUBLIC CURL::libcurl ${METEOSOURCE_JSONCPP_TARGET})

if(MSVC)
    target_compile_options(meteosource PRIVATE /W4)
else()
    target_compile_options(meteosource PRIVATE -Wall -Wextra)
endif()

add_executable(example example.cpp)
target_link_libraries(example PRIVATE meteosource)
