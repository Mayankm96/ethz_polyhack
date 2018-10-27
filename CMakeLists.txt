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

add_executable(takeoff_and_land
    src/takeoff_and_land.cpp
    src/beehaving_drones/BaseDrone.cpp
    src/beehaving_drones/Common.cpp
    src/beehaving_drones/CommonStruct.cpp
    src/trajectories/generate_trajectories.cc
)

target_link_libraries(takeoff_and_land
    dronecore
    dronecore_telemetry
    dronecore_action
)
