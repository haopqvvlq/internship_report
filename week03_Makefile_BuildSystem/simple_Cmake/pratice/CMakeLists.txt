cmake_minimum_required(VERSION 3.10)

project(MyProject C)

add_executable(main
    main.c
    src/quanly.c
)

target_include_directories(main PRIVATE
    inc
)