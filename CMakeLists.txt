
cmake_minimum_required(VERSION 3.10)

project(SVE_EXAMPLE C CXX)

if (BUILD_WITH_SVE2)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=armv8-a+sve2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv8-a+sve2")
elseif (BUILD_WITH_SVE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=armv8-a+sve")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv8-a+sve")
endif()
#set(CMAKE_BUILD_TYPE "Release")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

#add_compile_options(-save-temps)

add_executable(c_neon_sve c_neon_sve.cpp)


