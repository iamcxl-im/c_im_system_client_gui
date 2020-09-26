cmake_minimum_required(VERSION 3.17)
project(IMGUI C)

set(CMAKE_C_STANDARD 99)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK2 REQUIRED gtk+-2.0)

include_directories(${GTK2_INCLUDE_DIRS})
link_directories(${GTK2_LIBRARY_DIRS})

add_definitions(${GTK2_CFLAGS_OTHER})

add_executable(IMGUI cJSON.c cJSON.h message.c message.h mess_util.c mess_util.h main.c)
target_link_libraries(IMGUI ${GTK2_LIBRARIES})
target_link_libraries(IMGUI -lpthread -lm)