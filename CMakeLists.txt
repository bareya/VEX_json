cmake_minimum_required(VERSION 3.8)
project(VEX_json CXX)

find_package(Houdini PATHS ${HFS}/toolkit/cmake REQUIRED)

add_library(${PROJECT_NAME} SHARED
        src/json.h
        src/VEX_convert.h
        src/VEX_import.h
        src/VEX_jsonkeys.h
        src/VEX_json.cpp
        src/VEX_jsonkeys.cpp
        src/VEX_jsontype.cpp
        src/VEX_jsonutils.cpp
        src/VEX_jsonvalue.cpp
        )
target_link_libraries(${PROJECT_NAME} PRIVATE Houdini)

install(TARGETS ${PROJECT_NAME} DESTINATION "~/houdini$ENV{HOUDINI_MAJOR_RELEASE}.$ENV{HOUDINI_MINOR_RELEASE}/dso/vex/")
