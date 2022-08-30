cmake_minimum_required(VERSION 3.5.1)

project(net_shooter LANGUAGES C CXX VERSION 1)

# Config

if(EXISTS "${CMAKE_SOURCE_DIR}/LocalConfig.cmake")
    include("${CMAKE_SOURCE_DIR}/LocalConfig.cmake")
else()
    message(FATAL_ERROR "LocalConfig.cmake has not been found. Please, put it into project root directory.")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

message("Build type: " "${CMAKE_BUILD_TYPE}")


# Seek QT

find_package(Qt5 5.9
    COMPONENTS
    Core
    REQUIRED
    )

if(Qt5_FOUND)
    message("Qt5 version: " ${Qt5_VERSION})
    message("Qt5 core found: " ${Qt5Core_LIBRARIES})
endif()

# Seek Boost

find_package(Boost
    REQUIRED
    COMPONENTS
    regex
    program_options
    system
    filesystem
    thread
    log
    log_setup    
    )

# Almaz

if(NOT Almaz_FOUND)

    if(EXISTS ${ALMAZ_ROOT})
        list(APPEND CMAKE_PREFIX_PATH "${ALMAZ_ROOT}/lib/cmake/almaz")
    endif()

    find_package(Almaz CONFIG
        COMPONENTS
        logger
        misc
        REQUIRED
        )

    message("Almaz_INCLUDE_DIRS: " ${Almaz_INCLUDE_DIRS})
endif()


# Seek GTest

if(BUILD_TESTING)

    find_package(GTest REQUIRED)

    if(GTest_FOUND)
        message("Found the following GTest libraries: " ${GTEST_LIBRARIES})
        message("Found the following GTest include dirs: " ${GTEST_INCLUDE_DIRS})
    endif()

endif()

# Variables

set(NET_SHOOTER_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Subdirs

add_subdirectory(client)
add_subdirectory(server)


