cmake_minimum_required(VERSION 2.8)

project(FreedomGunduan)
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src/)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/)
set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib/)

set(SRC_SHADER 
    ${SRC_DIR}shaders/DSPhong_Material.vert
    ${SRC_DIR}shaders/DSPhong_Material.frag
    ${SRC_DIR}shaders/skybox.vert
    ${SRC_DIR}shaders/skybox.frag
    ${SRC_DIR}shaders/gray.vert
    ${SRC_DIR}shaders/gray.frag
    ${SRC_DIR}shaders/uniform.vert
    ${SRC_DIR}shaders/uniform.frag
    ${SRC_DIR}shaders/gaussian.vert
    ${SRC_DIR}shaders/gaussian.frag
    ${SRC_DIR}shaders/simple.vert
    ${SRC_DIR}shaders/simple.frag
    ${SRC_DIR}shaders/diamond_surface.vert
    ${SRC_DIR}shaders/diamond_surface.frag)

include_directories(${INCLUDE_DIR})

add_Definitions("-D_XKEYCHECK_H")

add_executable(FreedomGunduan
    ${SRC_DIR}main.h
    ${SRC_DIR}LoadShaders.h
    ${SRC_DIR}objloader.hpp
    ${SRC_DIR}vgl.h

    ${SRC_DIR}main.cpp
    ${SRC_DIR}objloader.cpp
    ${SRC_DIR}targa.cpp

    ${SRC_SHADER}
)
source_group("shaders" FILES ${SRC_SHADER})

target_link_libraries(FreedomGunduan
    ${LIB_DIR}freeglut_static.lib
    ${LIB_DIR}freeglut_staticd.lib
    ${LIB_DIR}glew32s.lib
    ${LIB_DIR}glew32sd.lib
    ${LIB_DIR}vermilion32.lib
    ${LIB_DIR}vermilion32_d.lib
    ${LIB_DIR}vermilion64.lib
    ${LIB_DIR}vermilion64_d.lib)
    