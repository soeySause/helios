cmake_minimum_required(VERSION 3.23)
project(helios)
include(FetchContent)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenSSL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Boost REQUIRED COMPONENTS serialization)

add_executable(heliosTest test/heliosTest1.cpp)
target_link_libraries(heliosTest helios)

add_library(${PROJECT_NAME} STATIC src/discordClasses/application/application.cpp include/discordClassses/application.hpp src/discordClasses/team/team.cpp include/discordClassses/team.hpp src/discordClasses/user/user.cpp include/discordClassses/user.hpp src/discordClasses/applicationRoleConnectionMetadata/applicationRoleConnectionMetadata.cpp include/discordClassses/applicationRoleConnectionMetadata.hpp include/discordClassses/discordClasses.hpp src/discordClasses/auditLog/auditLog.cpp include/discordClassses/auditLog.hpp src/discordClasses/channel/channel.cpp include/discordClassses/channel/channel.hpp src/discordClasses/channel/message.cpp include/discordClassses/channel/message.hpp src/discordClasses/channel/embed.cpp include/discordClassses/channel/embed.hpp src/discordClasses/channel/attachment.cpp include/discordClassses/channel/attachment.hpp)
target_sources(${PROJECT_NAME} PRIVATE
        src/client.cpp
        src/session.cpp
        include/session.hpp
        src/request.cpp
        include/request.hpp
        src/sharding.cpp
        src/event.cpp
        include/heliosException.hpp
        )

target_link_libraries(${PROJECT_NAME} OpenSSL::SSL)
target_link_libraries(${PROJECT_NAME} nlohmann_json)
target_link_libraries(helios Boost::serialization)


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

