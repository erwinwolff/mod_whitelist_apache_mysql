cmake_minimum_required(VERSION 3.10)

project(mod_whitelist VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES
    src/*.h
    src/*.cpp
)

add_executable(mod_whitelist ${SOURCES})
