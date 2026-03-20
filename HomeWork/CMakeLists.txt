cmake_minimum_required(VERSION 3.10)
project(CodeForStudent_NewVersion C)

set(CMAKE_C_STANDARD 99)

add_executable(CodeForStudent_NewVersion
        main.c
        imageIO.c
        IMG_Processing.c
        IMG_Processing.h
        Matrx_Calculate.c
        Matrx_Calculate.h
)

configure_file(lena_in.bmp ${CMAKE_BINARY_DIR}/lena_in.bmp COPYONLY)