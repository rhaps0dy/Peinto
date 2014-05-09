cmake_minimum_required(VERSION 2.8)

project(Peinto)

#CXXFLAGS
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG -D_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g")

add_definitions("-Wall -I/usr/local/include")

string(COMPARE EQUAL "${CMAKE_BUILD_TYPE}" "" EMPTY_BUILD_TYPE)
if(EMPTY_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "RELEASE")
endif(EMPTY_BUILD_TYPE)

# Find the SDL2 configuration file
set(SDL2_DIR ${CMAKE_SOURCE_DIR})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(SDL2 REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS}
	${SDL2_INCLUDE_DIRS})

add_subdirectory(src)
