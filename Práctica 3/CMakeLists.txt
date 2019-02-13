cmake_minimum_required(VERSION 2.8) 
project(Practica3 CXX)
set(EXECUTABLE_OUTPUT_PATH "../bin")
set(CMAKE_CXX_FLAGS "-g -std=c++11")

#####################################################################
# Buscar glut y openGL
#####################################################################
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

#####################################################################
# SRC e includes
#####################################################################
file(GLOB SRC "src/*.cc")
include_directories("include")

#####################################################################
# Ejecutable y librerías
#####################################################################
add_executable(Practica3 ${SRC})
target_link_libraries(Practica3 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
