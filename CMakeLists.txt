cmake_minimum_required (VERSION 2.8)

# define a macro that helps defining an option
include("cmake/macro.cmake")

# project name
set(PROJECT_NAME "lab")
project (${PROJECT_NAME})

# setup version numbers
set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)


# add the  header path
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ideas)

# add an option for building the API documentation
set_option(LAB_BUILD_DOC TRUE BOOL "TRUE to generate the API documentation, FALSE to ignore it")

set_option(LAB_BUILD_LUA TRUE BOOL "TRUE to generate LUA tests, FALSE to ignore it")
set_option(LAB_BUILD_EVENTS TRUE BOOL "TRUE to generate LUA tests, FALSE to ignore it")

set(LIBS "")

set(DEFINES "")
set(WARNING "-Wall")
set(FLAGS "-g -std=c++0x")
set(CMAKE_CXX_FLAGS "${DEFINES} ${WARNING} ${FLAGS}")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ideas)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ideas)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ideas)
set(LIB_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ideas)

# add the subdirectories
add_subdirectory(ideas)

#build doc
if(LAB_BUILD_DOC)
    add_subdirectory(doc)
endif()

