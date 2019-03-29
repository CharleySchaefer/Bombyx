#https://cmake.org/cmake-tutorial/

project (ZiltoidLIB)
cmake_minimum_required (VERSION 2.6)
#cmake_minimum_required(VERSION 2.8)



#version number
set (ZiltoidLIB_VERSION_MAJOR 1)
set (ZiltoidLIB_VERSION_MINOR 0)
#set (ZiltoidLIB_VERSION_PATCH 0)
#set (ZiltoidLIB_VERSION_TWEAK 0)


include_directories("lib")
set(ZiltoidLIB_SRCS ReadTextFiles/ReadTextFiles.c StringOperations/StringOperations.c RandomNumbers/RandomNumbers.c Interpolation/Interpolation.c  Fourier/Fourier.c   StructureFactor/StructureFactor.c Mathematics/Mathematics.c Mathematics/DoolittleDeterminant/DoolittleDeterminant.c )
#add_executable (ZiltoidLIB ${ZiltoidLIB_SRCS})
#target_link_libraries(ZiltoidLIB m)
