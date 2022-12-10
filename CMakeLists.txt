cmake_minimum_required(VERSION 3.20)
project(ConstantIndexVector VERSION 1.0.0 LANGUAGES CXX)

add_executable(test test/test.cpp lib/catch2/catch_amalgamated.cpp)
set_property(TARGET test PROPERTY CXX_STANDARD 17)
target_include_directories(test PRIVATE "include" "lib")
