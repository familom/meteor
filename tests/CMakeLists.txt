cmake_minimum_required(VERSION 2.8)

project(meteor_test)

set(METEOR_TESTS_ROOT ${METEOR_ROOT}/tests)

file(GLOB METEOR_TESTS_SRCS ${METEOR_TESTS_ROOT}/src/*.cpp)

include_directories(
    ${METEOR_ROOT}/src
    ${Boost_INCLUDE_DIRS}
    ${GTest_INCLUDE_DIRS}
)

link_directories(
    ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

# Compile the application
add_executable(meteor_test
    ${METEOR_TESTS_SRCS}
)

target_link_libraries(meteor_test
    meteor
    ${Boost_LIBRARIES}
    ${GTest_LIBRARIES}
)

# Debug binaries will have 'd' postfix
set_target_properties(meteor_test PROPERTIES DEBUG_POSTFIX "d")

# Don't use CTest to preserve gtest original output
add_custom_target(check COMMAND meteor_test DEPENDS meteor_test)
