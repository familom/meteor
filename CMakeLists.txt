cmake_minimum_required(VERSION 2.8)

project(meteor)

# Project options
option(WithTests "Specifies whether to build tests" ON)
option(UseStaticBoost "Forces Boost static libraries usage" OFF)
option(MtTests "Enables pthreads usage for gtest" OFF)

set(METEOR_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(TESTS_DIR ${METEOR_ROOT}/tests)
set(THIRD_PARTY_DIR ${METEOR_ROOT}/3rdparty)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${METEOR_ROOT}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${METEOR_ROOT}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${METEOR_ROOT}/bin)

# Check compiler and configure options
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)
check_cxx_compiler_flag("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

if (COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif (COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else ()
    message(STATUS
        "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. "
        "Please use a different C++ compiler."
    )
endif ()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    message(STATUS "Supported compiler used: GCC")
    add_definitions("${CMAKE_CXX_FLAGS}")
    add_definitions("-Wall -Wextra -Werror -pedantic -pedantic-errors")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    message(STATUS "Supported compiler used: Clang")
    add_definitions("${CMAKE_CXX_FLAGS}")
    add_definitions("-Wall -Wextra -Werror -pedantic -pedantic-errors")
else ()
    message("Unsupported compiler used: ${CMAKE_CXX_COMPILER_ID}")
endif ()

# Find Boost
set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.50.0 COMPONENTS
    REQUIRED filesystem
)
include_directories(${Boost_INCLUDE_DIRS})

# Sources
set(METEOR_SRCS
    ${METEOR_ROOT}/src/cluster/node.h
    ${METEOR_ROOT}/src/cluster/resources_tracker.h
    ${METEOR_ROOT}/src/cluster/resources_tracker.cpp
)

# Compile the library
add_library(meteor
    ${METEOR_SRCS}
)

include_directories(${METEOR_ROOT})

# Add dependencies
target_link_libraries(meteor
    ${Boost_LIBRARIES}
)

# Debug binaries will have 'd' postfix
set_target_properties(meteor PROPERTIES DEBUG_POSTFIX "d")

# Build tests on-demand
if (WithTests)
    if (NOT MtTests)
        set(gtest_disable_pthreads ON)
    endif ()

    add_subdirectory(${THIRD_PARTY_DIR}/gtest-1.7.0)

    set(GTest_INCLUDE_DIRS ${THIRD_PARTY_DIR}/gtest-1.7.0/include)
    set(GTest_LIBRARIES gtest gtest_main)
    add_subdirectory(${TESTS_DIR})
endif ()
