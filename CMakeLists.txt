cmake_minimum_required(VERSION 3.18)
project(LiveBacktester CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR})

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    core/**/*.cpp
    core/**/*.hpp
    data/*.cpp
    main.cpp
)

add_executable(backtester ${SOURCES})
