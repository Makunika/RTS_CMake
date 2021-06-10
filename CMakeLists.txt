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

add_subdirectory(libs/freetype)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_executable(RTS src/main.cpp src/stb_image.h src/stb_image.cpp src/glad.c
        src/camera/Camera.h src/models/Mesh.h src/models/Model.h
        src/controls/Controls.h src/controls/Controls.cpp src/shaders/ShaderUtils.h src/shaders/Shader.h src/shaders/Shader.cpp
        src/state/State.h src/textures/Texture.h src/textures/TextureCubeMap.h src/textures/TextureOne.h
        src/utils/Utils.h src/window/Window.h src/game/Game.cpp src/game/Game.h src/game/objects/base/GameObject.cpp src/game/objects/base/GameObject.h src/game/objects/gameobjects/Tank.cpp src/game/objects/gameobjects/Tank.h src/game/Collider.h src/game/Collider.cpp src/window/MousePicker.cpp src/window/MousePicker.h src/window/TextRender.cpp src/window/TextRender.h src/resource/ResourceManager.cpp src/resource/ResourceManager.h src/state/GameState.h src/controls/GameControls.cpp src/controls/GameControls.h src/light/Light.h src/light/PointLight.cpp src/light/PointLight.h src/light/SpotLight.cpp src/light/SpotLight.h src/light/DirLight.cpp src/light/DirLight.h src/state/LightState.h src/floor/Floor.cpp src/floor/Floor.h src/game/objects/base/StaticObject.cpp src/game/objects/base/StaticObject.h src/game/objects/static/Tree.cpp src/game/objects/static/Tree.h src/shadow/Shadow.cpp src/shadow/Shadow.h src/game/objects/static/Home.cpp src/game/objects/static/Home.h src/game/objects/static/Lamp.cpp src/game/objects/static/Lamp.h)

target_link_libraries(RTS opengl32 glfw assimp freetype ${GLFW_LIBRARIES})