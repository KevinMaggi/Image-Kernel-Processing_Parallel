cmake_minimum_required(VERSION 3.20)
project(Image_Kernel_Processing_OpenMP)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")

add_executable(Image_Kernel_Processing_OpenMP main.c Image.h Image.c Utils.h Utils.c stb_image.h stb_image_write.h Kernel.c Kernel.h Processing.c Processing.h)
target_link_libraries(Image_Kernel_Processing_OpenMP m)