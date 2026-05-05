cmake_minimum_required(VERSION 3.10)

project(GRAFIKA LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

find_package(glfw3 REQUIRED)

add_library(glad STATIC
    src/glad.c
)

target_include_directories(glad PUBLIC include)

add_executable(GRAFIKA
    src/main.cpp
    src/Shader.cpp
    src/Mesh.cpp
    src/Texture.cpp
    src/Camera.cpp
    src/Scene.cpp
    src/ShadowMap.cpp
    src/glad.c
    src/tiny_obj_loader.cc
    src/stb_image_impl.c
)

target_link_libraries(GRAFIKA
    glad
    glfw
    "-framework OpenGL"
)
