#==========================================
#Project options
#==========================================
cmake_minimum_required(VERSION 2.8)

#==========================================
#Project name
#==========================================
project(3DGraphics_teacher)

#==========================================
#Building options
#==========================================

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -fopenmp")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

#==========================================
#Libraries path : glm, sfml, glew, opengl, freetype
#==========================================

set(GLM_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/extlib/glm-0.9.7.1" CACHE PATH "glm")
include_directories(${GLM_INCLUDE_DIRS})

set(GLEW_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/extlib/glew-1.13.0/include" CACHE PATH "glew")
set(GLEW_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/extlib/glew-1.13.0/lib/libGLEW.so" CACHE FILEPATH "glew")
include_directories(${GLEW_INCLUDE_DIRS})

set(SFML_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/extlib/sfml-2.3.2/include/" CACHE PATH "sfml")
set(SFML_SYSTEM_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/extlib/sfml-2.3.2/build/lib/libsfml-system.so" CACHE FILEPATH "sfml")
set(SFML_WINDOW_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/extlib/sfml-2.3.2/build/lib/libsfml-window.so" CACHE FILEPATH "sfml")
set(SFML_GRAPHICS_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/extlib/sfml-2.3.2/build/lib/libsfml-graphics.so" CACHE FILEPATH "sfml")
include_directories(${SFML_INCLUDE_DIRS})

set(FREETYPE_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/extlib/freetype-2.5.5/include" CACHE PATH "freetype")
set(FREETYPE_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/extlib/freetype-2.5.5/build/libfreetype.a" CACHE FILEPATH "freetype")
include_directories(${FREETYPE_INCLUDE_DIRS})

set(TINYOBJLOADER_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/extlib/tinyobjloader/" CACHE PATH "tinyobjloader")
set(TINYOBJLOADER_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/extlib/tinyobjloader/build/libtinyobjloader.a" CACHE FILEPATH "tinyobjloader")
include_directories(${TINYOBJLOADER_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)

#==============================================
#Project sources : src, include, shader, exe
#==============================================
file(
    GLOB_RECURSE
    HEADER_FILES
    include/*.hpp
    )

file(
    GLOB_RECURSE
    SOURCE_FILES
    src/*.cpp
    )

set(
    GLOB_RECURSE
    SHADER_FILES
    shaders/*.glsl
    )

set(EXECUTABLE_NAME main)

#==============================================
#Project executable definition
#==============================================
add_executable(${EXECUTABLE_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${SHADER_FILES})

#==============================================
#Linking with libraries
#==============================================
if (OPENGL_FOUND)
    target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_LIBRARIES})
    target_link_libraries(${EXECUTABLE_NAME} m)  # if you use maths.h
endif()

target_link_libraries(${EXECUTABLE_NAME} ${GLEW_LIBRARIES})
target_link_libraries(${EXECUTABLE_NAME} ${FREETYPE_LIBRARIES})
target_link_libraries(${EXECUTABLE_NAME} ${SFML_SYSTEM_LIBRARIES})
target_link_libraries(${EXECUTABLE_NAME} ${SFML_WINDOW_LIBRARIES})
target_link_libraries(${EXECUTABLE_NAME} ${SFML_GRAPHICS_LIBRARIES})
target_link_libraries(${EXECUTABLE_NAME} ${TINYOBJLOADER_LIBRARIES})

