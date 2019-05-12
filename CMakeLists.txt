cmake_minimum_required(VERSION 3.0)

set(CMAKE_TOOLCHAIN_FILE cmake/sdcc-generic.cmake) # path to sdcc-generic.cmake toolchain file.
set(CMAKE_MODULE_PATH .) # path to cmake files directory.
#set(STM8_StdPeriph_DIR ../stm8s-sdcc-template/STM8S_StdPeriph_Lib/Libraries/STM8S_StdPeriph_Driver) # path to StdPeriph directory.
set(STM8_CHIP stm8s103f3) # stm8 chip name, e.g. stm8l152c6 or stm8s105k4
set(CMAKE_C_COMPILER_WORKS 1)

project(cap-tester C)

add_definitions(-DSTM8S103 -DF_CPU=2000000)

include(../stm8-sdcc-cmake/cmake/sdcc-stm8s.cmake)

message(STATUS CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE: ${CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE})

set(std-periph /home/user/Project/IoT/stm8/stm8s-sdcc-template/STM8S_StdPeriph_Lib/Libraries/STM8S_StdPeriph_Driver)

include_directories(
  /usr/local/share/sdcc/include
  /home/user/Project/IoT/stm8/stm8-sdcc-examples
  /home/user/Project/IoT/stm8/stm8s-sdcc-template/Inc
  ${std-periph}/inc
#  /home/user/Project/IoT/stm8/stm8-sdcc-cmake/StdPeriph/STM8S/Libraries/STM8S_StdPeriph_Driver/inc/
#/home/user/Project/IoT/stm8/stm8-sdcc-cmake/StdPeriph/
)

#DEFINES = -D$(COMPILER) -D$(MCU) -DUSE_STDPERIPH_DRIVER

#cmake -DCMAKE_TOOLCHAIN_FILE=<path_to_sdcc-generic.cmake> -DCMAKE_MODULE_PATH=<path_to_project_cmake_dir> -DSTM8_CHIP=<chip name> -DSTM8_StdPeriph_DIR=<path to std periph> -G"MinGW Makefiles" <path_to_source_dir>
find_package(StdPeriph COMPONENTS gpio)
file(GLOB src
  main.*
  analog.*
  serial.*
  util.*
  gpio.*
  stm8.h
  cmake/*.cmake
  )
add_executable(${PROJECT_NAME} ${src})

#add_custom_target(Resource SOURCES readme.txt)

#Important compiler options for STM8 developers include:

#-c to compile into object files to be linked later
#--std-c99 for compilation in C99 mode (some C99 features, e.g. variable-length arrays
# are not yet supported in sdcc though)
#--opt-code-size for optimization for code size
#--max-allocs-per-node to select the optimization level. the default value is 3000.
# Higher values result in more optimized code, longer compiler runtime,
# and higher memory usage during compilation.
