cmake_minimum_required(VERSION 3.10)

# set the project name
project(modbus)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR} src/win32)
SET(GCC_COVERAGE_LINK_FLAGS    "-lwsock32 -lws2_32")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

add_library(modbus STATIC  src/win32/config.h src/modbus.c src/modbus.h src/modbus-data.c src/modbus-private.h src/modbus-rtu.c src/modbus-rtu.h src/modbus-rtu-private.h src/modbus-tcp.c src/modbus-tcp.h src/modbus-tcp-private.h src/modbus-version.h src/win32/config.h)

install(TARGETS modbus DESTINATION ${PROJECT_SOURCE_DIR}/lib)
install(FILES src/*.h DESTINATION ${PROJECT_SOURCE_DIR}/include)