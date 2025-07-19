cmake_minimum_required(VERSION 3.10)
project(udemy_1_initial)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

add_definitions("-Wall" "-g")

set(EXECUTABLES
    ${PROJECT_NAME}
    builder
    factory
    solid
    prototype
    singleton
    adapter
    bridge
)

foreach(EXECUTABLE IN LISTS EXECUTABLES)
    if(EXECUTABLE STREQUAL ${PROJECT_NAME})
        set(SOURCE_FILE ${PROJECT_SOURCE_DIR}/src/main.cpp)
    else()
        set(SOURCE_FILE ${PROJECT_SOURCE_DIR}/src/design_patterns/${EXECUTABLE}.cpp)
    endif()
    add_executable(${EXECUTABLE} ${SOURCE_FILE})
    target_compile_features(${EXECUTABLE} PUBLIC cxx_std_20)
endforeach()