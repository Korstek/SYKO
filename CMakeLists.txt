cmake_minimum_required(VERSION 3.10)
project(SYKO)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(SYKO
        f_add1.c
        f_rjmp.c
        file_code.bin
        file_counter.bin
        file_data.bin
        file_pc.bin
        gpio.c
        gpio.h
        interpreter.c
        interpreter.h
        interrupt.c
        interrupt.h
        main.c
        makefile
        mem_abs.c
        mem_abs.h
        README.md
        spi.c
        spi.h
        types.h f_in.c f_out.c f_sbrs.c f_ret.c f_sbi.c)
