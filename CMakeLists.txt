cmake_minimum_required(VERSION 3.23)
project(helios)
include(FetchContent)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenSSL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Boost REQUIRED COMPONENTS serialization)
find_package(Boost REQUIRED COMPONENTS filesystem)

add_executable(heliosTest test/heliosTest1.cpp)
target_link_libraries(heliosTest helios)

add_library(${PROJECT_NAME} SHARED src/discordClasses/guild/guild.cpp include/discordClasses/guild/guild.hpp src/discordClasses/voice.cpp include/discordClasses/voice.hpp include/discordClasses/invite.hpp src/discordClasses/invite.cpp src/discordClasses/guildScheduledEvent.cpp include/discordClasses/guildScheduledEvent.hpp src/discordClasses/eventFields/presence.cpp include/discordClasses/eventFields/presence.hpp src/discordClasses/eventFields/activity.cpp include/discordClasses/eventFields/activity.hpp "src/discordClasses/stageInstance.cpp" "include/discordClasses/stageInstance.hpp" src/discordClasses/autoModeration.cpp include/discordClasses/autoModeration.hpp src/discordClasses/guildTemplate.cpp include/discordClasses/guildTemplate.hpp src/discordClasses/webhook.cpp include/discordClasses/webhook.hpp src/discordClasses/interactions/applicationCommands.cpp include/discordClasses/interactions/applicationCommands.hpp src/rateLimit.cpp include/rateLimit.hpp)
target_sources(${PROJECT_NAME} PRIVATE
        src/client.cpp
        src/session.cpp
        include/session.hpp
        src/request.cpp
        include/request.hpp
        src/sharding.cpp
        src/event.cpp
        include/heliosException.hpp
        src/cache.cpp
        include/cache.hpp

        src/discordClasses/application.cpp
        include/discordClasses/application.hpp
        src/discordClasses/team.cpp
        include/discordClasses/team.hpp
        src/discordClasses/user.cpp
        include/discordClasses/user.hpp
        src/discordClasses/applicationRoleConnectionMetadata.cpp
        include/discordClasses/applicationRoleConnectionMetadata.hpp
        include/discordClasses/discordClasses.hpp
        src/discordClasses/auditLog.cpp
        include/discordClasses/auditLog.hpp
        src/discordClasses/channel/channel.cpp
        include/discordClasses/channel/channel.hpp
        src/discordClasses/channel/message.cpp
        include/discordClasses/channel/message.hpp
        src/discordClasses/channel/embed.cpp
        include/discordClasses/channel/embed.hpp
        src/discordClasses/channel/attachment.cpp
        include/discordClasses/channel/attachment.hpp
        src/discordClasses/guild/role.cpp
        include/discordClasses/guild/role.hpp
        src/discordClasses/guild/guildMember.cpp
        include/discordClasses/guild/guildMember.hpp
        src/discordClasses/sticker.cpp
        include/discordClasses/sticker.hpp
        src/discordClasses/emoji.cpp
        include/discordClasses/emoji.hpp
        src/discordClasses/interactions/interaction.cpp
        include/discordClasses/interactions/interaction.hpp
        src/discordClasses/interactions/messageComponent.cpp
        include/discordClasses/interactions/messageComponent.hpp
        )

target_link_libraries(${PROJECT_NAME} OpenSSL::SSL)
target_link_libraries(${PROJECT_NAME} nlohmann_json)
target_link_libraries(helios Boost::serialization)
target_link_libraries(helios Boost::filesystem)

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

