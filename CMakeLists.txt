cmake_minimum_required(VERSION 3.6)
project(FireworksDemo)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")

set(SRC_DIRECTORY "src")
set(LIB_GLAD_DIRECTORY "lib/glad")
set(LIB_GLFW_DIRECTORY "lib/glfw/lib-mingw-w64")
set(LIB_SOIL_DIRECTORY "lib/soil")
set(LIB_GLM_DIRECTORY "lib/glm")
set(LIB_FREETYPE_DIRECTORY "lib/freetype/lib")

include_directories(${LIB_GLAD_DIRECTORY}/include)
include_directories(${LIB_GLFW_DIRECTORY}/../include)
include_directories(${LIB_SOIL_DIRECTORY}/src)
include_directories(${LIB_GLM_DIRECTORY})
include_directories(${LIB_FREETYPE_DIRECTORY}/../include)
include_directories(${LIB_FREETYPE_DIRECTORY}/../include/freetype2)

link_directories(${LIB_GLFW_DIRECTORY})
link_directories(${LIB_FREETYPE_DIRECTORY})


add_library(SOIL STATIC
            ${LIB_SOIL_DIRECTORY}/src/stb_image_aug.c
            ${LIB_SOIL_DIRECTORY}/src/image_DXT.c
            ${LIB_SOIL_DIRECTORY}/src/image_helper.c
            ${LIB_SOIL_DIRECTORY}/src/SOIL.c)

add_library(glad STATIC
            ${LIB_GLAD_DIRECTORY}/src/glad.c)

set(SOURCE_FILES ${SRC_DIRECTORY}/main.cpp
                 ${SRC_DIRECTORY}/FireworksDemoEngine.cpp
                 ${SRC_DIRECTORY}/GLFWWrapper.cpp
                 ${SRC_DIRECTORY}/Shader.cpp
                 ${SRC_DIRECTORY}/ResourceManager.cpp
                 ${SRC_DIRECTORY}/Texture.cpp
                 ${SRC_DIRECTORY}/SpriteRenderer.cpp
                 ${SRC_DIRECTORY}/TimeManager.cpp
                 ${SRC_DIRECTORY}/Scene.cpp
                 ${SRC_DIRECTORY}/TextRenderer.cpp
                 ${SRC_DIRECTORY}/RenderToTexture.cpp
                 ${SRC_DIRECTORY}/ParticleSystem.cpp
                 ${SRC_DIRECTORY}/Firework.cpp
                 ${SRC_DIRECTORY}/Random.cpp
                 ${SRC_DIRECTORY}/PostEffects.cpp)

find_library(SOIL
             freetype
             opengl32
             glad
             glfw3)

add_executable(FireworksDemo ${SOURCE_FILES})



target_link_libraries(FireworksDemo SOIL
                                    freetype
                                    opengl32
                                    glad
                                    glfw3)

