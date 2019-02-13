cmake_minimum_required(VERSION 2.8) 
project(Practica4 CXX)
set(EXECUTABLE_OUTPUT_PATH "../bin")
set(CMAKE_CXX_FLAGS "-g -std=c++11")

#####################################################################
# Buscar librerias
#####################################################################
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

find_package(X11 REQUIRED)
include_directories(${X11_INCLUDE_DIR})

find_package(Threads REQUIRED)

#####################################################################
# SRC e includes
#####################################################################
file(GLOB SRC "src/*.cc")
include_directories("include")

#####################################################################
# Ejecutable y librerías
#####################################################################
add_executable(Practica4 ${SRC})
target_link_libraries(Practica4 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${X11_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
