cmake_minimum_required(VERSION 3.16)

project(MPS)

add_subdirectory(external/SDL)

add_executable(main
    src/main.cpp
    src/application.cpp
    src/material.cpp
    src/utils.cpp
)

target_include_directories(main
    PUBLIC
        include
)

target_link_libraries(main
    PRIVATE
        SDL3
)
