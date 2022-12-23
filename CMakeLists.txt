cmake_minimum_required(VERSION 3.23)
project(helios)

set(PROJECT_NAME helios)
set(CMAKE_CXX_STANDARD 20)

add_library(helios SHARED include/events.h)

target_sources(${PROJECT_NAME} PRIVATE src/client.cpp)

target_include_directories(${PROJECT_NAME}
        PRIVATE
            # where the library itself will look for its internal headers
            ${CMAKE_CURRENT_SOURCE_DIR}/src
        PUBLIC
            # where top-level project will look for the library's public headers
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            # where external projects will look for the library's public headers
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        )

if(DEFINED CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    message(
            STATUS
            "CMAKE_INSTALL_PREFIX is not set\n"
            "Default value: ${CMAKE_INSTALL_PREFIX}\n"
            "Will set it to ${CMAKE_SOURCE_DIR}/bin"
    )
    set(CMAKE_INSTALL_PREFIX
            "${CMAKE_SOURCE_DIR}/bin"
            CACHE PATH "Where the library will be installed to" FORCE
            )
else()
    message(
            STATUS
            "CMAKE_INSTALL_PREFIX was already set\n"
            "Current value: ${CMAKE_INSTALL_PREFIX}"
    )
endif()

set(public_headers
        include/client.h
        )
# note that ${public_headers} has to be in quotes
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${public_headers}")
