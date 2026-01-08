cmake_minimum_required(VERSION 3.29)
project(dcp C)

set(CMAKE_C_STANDARD 99)

add_library(dcpf STATIC
        src/dcpf.c
)

target_include_directories(dcpf PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(dcpf_example
        examples/example.c
)

target_link_libraries(dcpf_example PRIVATE dcpf)
