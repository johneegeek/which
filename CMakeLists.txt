cmake_minimum_required(VERSION 3.24)

project(which
    VERSION 4.0.0
    LANGUAGES CXX
)
set(CXXOPTS_CXX_STANDARD 17)

set( PROJECT_ROOT ${CMAKE_SOURCE_DIR} )
set( PROJECT_SRC_DIR ${PROJECT_ROOT}/src )


set(main-src 
    src/main.cpp
)

add_executable(which ${main-src})

target_include_directories(which
    PUBLIC
        ${PROJECT_SRC_DIR}
        ${PROJECT_SRC_DIR}/include
)