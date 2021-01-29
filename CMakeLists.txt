cmake_minimum_required(VERSION 3.10)


# set the project name
SET(THE_TARGET EmbeddedModbusGUI)
project(EmbeddedModbusGUI)

#find_library(LVGL_LIB lvgl HINTS ${PROJECT_SOURCE_DIR}/libLVGL/build)

find_package(SDL2 REQUIRED SDL2)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR} GUI/inc)

include_directories(${SDL2_INCLUDE_DIRS})
add_subdirectory(libmodbus )
add_subdirectory(libLVGL )

add_executable(EmbeddedModbusGUI main.c GUI/src/ao.c GUI/src/ai.c GUI/src/do.c GUI/src/di.c GUI/src/mainpage.c GUI/src/connection.c GUI/inc/ao.h GUI/inc/ai.h GUI/inc/do.h GUI/inc/di.h GUI/inc/mainpage.h GUI/inc/connection.h )

target_link_libraries(EmbeddedModbusGUI modbus lvgl SDL2 ws2_32)
#message(STATUS "LVGL_LIB: ${LVGL_LIB}")

target_include_directories(EmbeddedModbusGUI PUBLIC
                          "${PROJECT_SOURCE_DIR}/libLVGL"
                          "${PROJECT_SOURCE_DIR}/libmodbus/src"
                          )

install(TARGETS EmbeddedModbusGUI DESTINATION Debug)