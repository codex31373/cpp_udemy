cmake_minimum_required(VERSION 3.7)
project(udemy_1_initial)
#warrning levels defines send to GCC
#add_definitions("-Wall" "-g")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

set(MAIN_SRC ${PROJECT_SOURCE_DIR}/src/main.cpp)
set(DESIGN_PATTERNS_BUILDER_SRC ${PROJECT_SOURCE_DIR}/src/design_patterns/design_patterns_builder.cpp)

add_executable(${PROJECT_NAME} ${MAIN_SRC})
add_executable(design_patterns_builder ${DESIGN_PATTERNS_BUILDER_SRC})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_compile_features(design_patterns_builder PUBLIC cxx_std_20)






