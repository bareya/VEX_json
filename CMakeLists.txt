cmake_minimum_required(VERSION 2.8)

project(VEX_json CXX)

set(CMAKE_CXX_COMPILER g++-4.8)
#set(CMAKE_VERBOSE_MAKEFILE TRUE)

add_definitions(-std=c++11 -D_GNU_SOURCE -DLINUX -DAMD64 -m64 -fPIC -DSIZEOF_VOID_P=8 -DSESI_LITTLE_ENDIAN -DENABLE_THREADS -DUSE_PTHREADS -D_REENTRANT -D_FILE_OFFSET_BITS=64 -c -DGCC4 -DGCC3 -Wno-deprecated -std=c++11)
#add_definitions(-fvisibility=hidden)

INCLUDE_DIRECTORIES($ENV{HFS}/toolkit/include)
LINK_DIRECTORIES($ENV{HFS}/dsolib)

file(GLOB SOURCES "src/*.cpp")
add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_link_libraries(${PROJECT_NAME} HoudiniUT HoudiniGEO)

install(TARGETS ${PROJECT_NAME} DESTINATION "~/houdini$ENV{HOUDINI_MAJOR_RELEASE}.$ENV{HOUDINI_MINOR_RELEASE}/dso/vex/")
