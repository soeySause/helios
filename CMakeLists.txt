cmake_minimum_required(VERSION 3.23)
project(helios)
include(FetchContent)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED)
FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json
        UPDATE_DISCONNECTED 1
)

FetchContent_Populate(json)
include_directories(${json_SOURCE_DIR}/include/)

add_executable(heliosTest test/heliosTest1.cpp)
target_link_libraries(heliosTest helios)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
        PRIVATE
        src/client.cpp
        src/session.cpp
        include/session.hpp
        src/cache.cpp
        include/cache.hpp
        src/request.cpp
        include/request.hpp
        src/discordApiRequest.cpp
        include/discordApiRequest.hpp
        src/sharding.cpp
        src/event.cpp
        include/heliosException.hpp
        )

target_link_libraries(${PROJECT_NAME} OpenSSL::SSL)

target_include_directories(${PROJECT_NAME}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

set(public_headers
        include/client.hpp
        include/event.hpp
        include/sharding.hpp
        )

