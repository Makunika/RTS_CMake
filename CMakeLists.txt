cmake_minimum_required(VERSION 3.17)
project(RTS)

set(CMAKE_CXX_STANDARD 20)

INCLUDE_DIRECTORIES(
        ${GL_INCLUDE_DIR})

include_directories(includes)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(libs/glfw)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(libs/assimp)

add_executable(RTS src/main.cpp src/stb_image.h src/stb_image.cpp src/glad.c
        src/camera/Camera.h src/models/Mesh.h src/models/Model.h
        src/controls/Controls.h src/controls/Controls.cpp src/shaders/ShaderUtils.h src/shaders/Shader.h src/shaders/Shader.cpp
        src/state/State.h src/textures/Texture.h src/textures/TextureCubeMap.h src/textures/TextureOne.h
        src/utils/Utils.h src/window/Window.h)

target_link_libraries(RTS opengl32 glfw assimp ${GLFW_LIBRARIES})