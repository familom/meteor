cmake_minimum_required(VERSION 2.8)

project(meteor_simulator)

set(SIMULATOR_ROOT ${METEOR_ROOT}/tools/simulator)

SET(SIMULATOR_SRCS
    ${SIMULATOR_ROOT}/main.cpp
)

include_directories(
    ${METEOR_ROOT}/src
    ${METEOR_ROOT}/src/cluster
    ${Boost_INCLUDE_DIRS}
)

link_directories(
    ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

# Compile the application
add_executable(simulator
    ${SIMULATOR_SRCS}
)

target_link_libraries(simulator
    meteor
    ${Boost_LIBRARIES}
)

# Debug binaries will have 'd' postfix
set_target_properties(simulator PROPERTIES DEBUG_POSTFIX "d")

add_custom_target(simulate COMMAND simulator DEPENDS simulator)
