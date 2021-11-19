cmake_minimum_required(VERSION 3.10)
project(ConstantIndexVector VERSION 1.0.0 LANGUAGES CXX)

file(GLOB source_files
    "src/main.cpp"
    "include/index_vector.hpp"
)

set(SOURCES ${source_files})

# Set build type
if(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

add_executable(${PROJECT_NAME} ${WIN32_GUI} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE "include")
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
