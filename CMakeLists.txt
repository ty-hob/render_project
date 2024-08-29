cmake_minimum_required(VERSION 3.10)


add_library(vector src/vector.c)
add_library(scene src/rendeng/scene.c)
add_library(objs src/rendeng/objs.c)
add_library(models src/rendeng/models.c)
add_library(linalg src/rendeng/linalg.c)
add_library(image src/rendeng/image.c)
add_library(error src/rendeng/error.c)


project(rendeng VERSION 0.1)

add_executable(rendeng src/rendeng/main.c)

configure_file(include/rendeng/version.h.in include/rendeng/version.h)

target_include_directories(rendeng PUBLIC include include/rendeng)
target_link_libraries(rendeng PUBLIC vector scene objs models linalg image error)
