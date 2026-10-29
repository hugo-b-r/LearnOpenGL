
#include "DebugInfos.h"
#include "Scene.h"
#include <glad/glad.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>


#include <spdlog/spdlog.h>
// #define STB_IMAGE_IMPLEMENTATION
#include "../deps/stb_image.h"
#include <GLFW/glfw3.h>

// void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
//         ImGuiIO &io = ImGui::GetIO();
//         if (!io.WantCaptureMouse) {
//                 Game *scene =
//                     static_cast<Game *>(glfwGetWindowUserPointer(window));

//                 if (!scene->configuration.mouseFree) {
//                         float xoffset = (float)xpos - scene->camera.lastX;
//                         float yoffset =
//                             scene->camera.lastY -
//                             (float)ypos; // reversed since y-coordinates range
//                                          // from bottom to top
//                         scene->camera.lastX = (float)xpos;
//                         scene->camera.lastY = (float)ypos;

//                         const float sensitivity = 0.1f;
//                         xoffset *= sensitivity;
//                         yoffset *= sensitivity;

//                         scene->camera.m_yaw += xoffset;
//                         scene->camera.m_pitch += yoffset;

//                         if (scene->camera.m_pitch > 89.0f)
//                                 scene->camera.m_pitch = 89.0f;
//                         if (scene->camera.m_pitch < -89.0f)
//                                 scene->camera.m_pitch = -89.0f;

//                         glm::vec3 direction;
//                         direction.x =
//                             cos(glm::radians(scene->camera.m_yaw)) *
//                             cos(glm::radians(scene->camera.m_pitch));
//                         direction.y =
//                             sin(glm::radians(scene->camera.m_pitch));
//                         direction.z =
//                             sin(glm::radians(scene->camera.m_yaw)) *
//                             cos(glm::radians(scene->camera.m_pitch));
//                         scene->camera.m_frontPos =
//                             glm::normalize(direction);
//                         scene->camera.update();
//                 }
//         }
// }

// void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
//         Game *s = (Game *)glfwGetWindowUserPointer(window);
//         s->configuration.windowHeight = height;
//         s->configuration.windowWidth = width;
//         s->camera.updateProjViewMatrix();
//         glViewport(0, 0, width, height);
// }

// void error_callback(int error, const char *description) {
//         std::cerr << "Erreur GLFW (" << error << "): " << description
//                   << std::endl;
// }

// void processInput(GLFWwindow *window, Game &s) {
//         static float currentFrame, deltaTime, lastFrame;
//         currentFrame = (float)glfwGetTime();
//         deltaTime = currentFrame - lastFrame;
//         lastFrame = currentFrame;

//         const float camera_speed = s.configuration.camSpeedFactor * deltaTime;
//         if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
//                 s.camera.translate_forward(camera_speed);
//         }
//         if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
//                 s.camera.translate_left(camera_speed);
//         }
//         if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
//                 s.camera.translate_forward(-camera_speed);
//         }
//         if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
//                 s.camera.translate_right(camera_speed);
//         }
//         if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
//                 s.camera.translate(camera_speed * glm::vec3(0., -1., 0.));
//         }
//         if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
//                 s.camera.translate(camera_speed * glm::vec3(0., 1., 0.));
//         }
//         if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
//                 s.configuration.camSpeedFactor = 50;
//         } else {
//                 s.configuration.camSpeedFactor = 5;
//         }
// }

// void key_callback(GLFWwindow *window, int key, int scancode, int action,
//                   int mods) {
//         UNUSED(mods);
//         UNUSED(scancode);
//         Game *sc_ptr = (Game *)glfwGetWindowUserPointer(window);
//         if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
//                 // to be fixed : check opengl polygone mode using some sort of
//                 // app variable that has been linked to the gl window
//                 GLint polyg_mode;
//                 glGetIntegerv(GL_POLYGON_MODE, &polyg_mode);

//                 if (polyg_mode == GL_LINE) {
//                         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//                 } else {
//                         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//                 }
//         }
//         if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
//                 sc_ptr->configuration.mouseFree = !sc_ptr->configuration.mouseFree;
//                 if (sc_ptr->configuration.mouseFree) {
//                         glfwSetInputMode(window, GLFW_CURSOR,
//                                          GLFW_CURSOR_NORMAL);
//                 } else {
//                         glfwSetInputMode(window, GLFW_CURSOR,
//                                          GLFW_CURSOR_DISABLED);
//                 }
//         }
//         if (key == GLFW_KEY_F3 && action == GLFW_PRESS) {
//                 sc_ptr->configuration.showDebugInfos =
//                     !sc_ptr->configuration.showDebugInfos;
//                 spdlog::debug("Show or hide now debug infos");
//         }
// }

// void mouseButtonCallback(GLFWwindow *window, int button, int action,
//                           int mods) {
//         UNUSED(mods);
//         ImGuiIO &io = ImGui::GetIO();
//         if (!io.WantCaptureMouse) {
//                 Game *sc_ptr = (Game *)glfwGetWindowUserPointer(window);
//                 if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
//                         if (sc_ptr->configuration.mouseFree) {
//                                 sc_ptr->configuration.mouseFree =
//                                     !sc_ptr->configuration.mouseFree;
//                                 glfwSetInputMode(window, GLFW_CURSOR,
//                                                  GLFW_CURSOR_DISABLED);
//                         }
//                 }
//         }
// }
// int main() {


// #ifdef NDEBUG
//         spdlog::set_level(spdlog::level::info); // release mode
// #else
//         spdlog::set_level(spdlog::level::debug); //debug mode
// #endif
//         Game scene;
//         scene.camera.position.y =
//             WorldGenerator::soilHeightAt(glm::vec2(0, 0)) + 1 + 1.8;
//         scene.camera.update();

//         // ----------------------
//         // Init glfw

//         glfwInit();
//         glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//         glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//         glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//         // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//         spdlog::info("GLFW has been initialized...");

//         glfwSetErrorCallback(error_callback);

//         GLFWwindow *window = glfwCreateWindow(scene.configuration.windowWidth,
//                                               scene.configuration.windowHeight,
//                                               "LearnOpenGL", NULL, NULL);
//         if (window == NULL) {
//                 spdlog::error("Failed to create GLFW window");
//                 glfwTerminate();
//                 return -1;
//         }
//         glfwMakeContextCurrent(window);
//         if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//                 spdlog::error("Failed to initialize GLAD");
//                 return -1;
//         }
//         spdlog::info("Window has been created");
//         glfwSetWindowUserPointer(window, &scene);
//         glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//         glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

//         glfwSetKeyCallback(window, key_callback);
//         glfwSetMouseButtonCallback(window, mouse_button_callback);
//         glfwSetCursorPosCallback(window, mouse_callback);
//         int frameBufferWidth, frameBufferHeight;
//         glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
//         glViewport(0, 0, frameBufferWidth, frameBufferHeight);
//         scene.configuration.windowHeight = frameBufferHeight;
//         scene.configuration.windowWidth = frameBufferWidth;

//         glfwSwapInterval(0);

//         DebugInfos debug_inf(window, scene);

//         // ----------------------
//         // Init imgui
//         // Setup Dear ImGui context

//         glEnable(GL_DEPTH_TEST);
//         glEnable(GL_CULL_FACE);
//         // glEnable(GL_BLEND);
//         // glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);

//         Shader mainShader("Sources/Shaders/textured_cube_vertex.glsl",
//                           "Sources/Shaders/textured_cube_fragment.glsl");

//         // Game variables
//         // create vertices of what will beglUseProgram(shaderProgram); shown
//         // 2. copy our vertices array in a buffer for OpenGL to useChun

//         ChunkRenderer cr;
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
//                         GL_NEAREST_MIPMAP_NEAREST);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

//         scene.world.load();

//         while (!glfwWindowShouldClose(window)) {
//                 glfwPollEvents();

//                 if (scene.configuration.showDebugInfos) {
//                         debug_inf.initFrame();
//                 }
//                 processInput(window, scene);

//                 glClearColor(0.63f, 0.87f, 1.f, 1.0f);
//                 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//                 scene.world.render(scene.camera, cr, debug_inf);

//                 if (scene.configuration.showDebugInfos) {
//                         debug_inf.drawFrame(scene);
//                 }

//                 glfwSwapBuffers(window);
//         }

//         // needed to delete before glfw terminate
//         debug_inf.terminate();
//         glfwTerminate();
//         return EXIT_SUCCESS;
// }
//
//
int main() {
        Game game;
        game.mainLoop();

        // put loop in window update, renderer update, etc.
        // create a game mainloop
        // call it from here
        //
        // checks that everything got initialized in the right order

}
