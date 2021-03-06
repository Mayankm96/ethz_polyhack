cmake_minimum_required(VERSION 2.8.12)

project(beehaving_drones)

set(DRONECODE_SRC ${HOME}/projects/DronecodeSDK)

if(NOT MSVC)
    add_definitions("-std=c++11 -Wall -Wextra -Werror")
else()
    add_definitions("-std=c++11 -WX -W2")
    include_directories(${DRONECODE_SRC}/install/include)
    link_directories(${DRONECODE_SRC}/install/lib)
endif()

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  include
)

##################################

add_executable(takeoff_and_land
    src/takeoff_and_land.cpp
    src/beehaving_drones/BaseDrone.cpp
    src/beehaving_drones/Common.cpp
    src/beehaving_drones/Console.cpp
    src/beehaving_drones/CommonStruct.cpp
)

target_link_libraries(takeoff_and_land
    dronecore
    dronecore_telemetry
    dronecore_mission
    dronecore_action
)

##################################

add_executable(follow_mission
    src/follow_mission.cpp
    src/beehaving_drones/BaseDrone.cpp
    src/beehaving_drones/Common.cpp
    src/beehaving_drones/Console.cpp
    src/beehaving_drones/CommonStruct.cpp
    src/trajectories/generate_trajectories.cpp
)

target_link_libraries(follow_mission
    dronecore
    dronecore_telemetry
    dronecore_mission
    dronecore_action
)

##################################

add_executable(follow_mission_from_file
    src/follow_mission_from_file.cpp
    src/beehaving_drones/BaseDrone.cpp
    src/beehaving_drones/Common.cpp
    src/beehaving_drones/Console.cpp
    src/beehaving_drones/CommonStruct.cpp
    src/trajectories/generate_trajectories.cpp
)

target_link_libraries(follow_mission_from_file
    dronecore
    dronecore_telemetry
    dronecore_mission
    dronecore_action
)

##################################

add_executable(test_behavior
    src/test_behavior.cpp
    src/beehaving_drones/BaseDrone.cpp
    src/beehaving_drones/Common.cpp
    src/beehaving_drones/Console.cpp
    src/beehaving_drones/CommonStruct.cpp
    src/trajectories/generate_trajectories_with_goals.cpp
)

target_link_libraries(test_behavior
    dronecore
    dronecore_telemetry
    dronecore_mission
    dronecore_action
)
