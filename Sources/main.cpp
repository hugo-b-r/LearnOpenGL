
#include "DebugInfos.h"
#include "Scene.h"
#include <glad/glad.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>


#include <spdlog/spdlog.h>
// #define STB_IMAGE_IMPLEMENTATION
#include "../deps/stb_image.h"
#include <GLFW/glfw3.h>


///
/// \mainpage
/// # Introduction
/// LearnOpenGL is a basic OpenGL project that have the goal of :
/// - Making me learn OpenGL in a big project
/// - do this while reproducing the well known, yet very interesting mechanics of minecraft
///
/// # Installation
///
/// These are the commands I am using.
/// 
/// ```bash
/// cmake -B build . -DCMAKE_BUILD_TYPE=Release -S .
/// ```
/// Then, build: 
/// ```bash
/// cmake --build build --parallel <number of core you want> --target learnopengl
/// ```
///
/// In the future, I plan on creating rpm, deb, nsis windows installer and mac packages.
/// 
/// # Functionnalities
/// - [x] World generation
/// - [x] Chunk management
/// - [x] OpenGL renderer
/// - [ ] Vulkan renderer
/// - [ ] World Saving
/// - [ ] Structure generation (trees, villages, towers.....)
/// - [ ] Water
/// - [ ] Better shaders
/// - [ ] Buttons and ui elements
/// - [ ] configuration file
///
///
//


int main() {
        Game game;
        game.mainLoop();
}
