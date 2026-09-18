#include "OpenGLWindow.h"
#include <cstdlib>
#include <iostream>
#include <imgui.h>
#include "../deps/imgui/imgui.h"

#include "GLFW/glfw3.h"
#include "utils.h"
#include "Scene.h"
#include "DebugInfos.h"

#include <spdlog/spdlog.h>

OpenGLWindow::OpenGLWindow(Configuration* configuration, Game*game): game(game) {


        


#ifdef NDEBUG
        spdlog::set_level(spdlog::level::info); // release mode
#else
        spdlog::set_level(spdlog::level::debug); //debug mode
#endif
        // Game scene;
        // scene.camera.position.y =
            // WorldGenerator::soilHeightAt(glm::vec2(0, 0)) + 1 + 1.8;
        // scene.camera.update();

        // ----------------------
        // Init glfw

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        spdlog::info("GLFW has been initialized...");

        glfwSetErrorCallback(&OpenGLWindow::errorCallback);

        window = glfwCreateWindow(configuration->windowWidth,
                                              configuration->windowHeight,
                                              "LearnOpenGL", NULL, NULL);
        if (window == NULL) {
                spdlog::error("Failed to create GLFW window");
                glfwTerminate();
                exit(EXIT_FAILURE);
        }
        glfwMakeContextCurrent(window);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                spdlog::error("Failed to initialize GLAD");
                exit(EXIT_FAILURE);
        }
        spdlog::info("Window has been created");
        glfwSetWindowUserPointer(window, game);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetFramebufferSizeCallback(window, &OpenGLWindow::framebufferSizeCallback);

        glfwSetKeyCallback(window, &OpenGLWindow::keyCallback);
        glfwSetMouseButtonCallback(window, &OpenGLWindow::mouseButtonCallback);
        glfwSetCursorPosCallback(window, &OpenGLWindow::mouseCallback);
        int frameBufferWidth, frameBufferHeight;
        glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
        glViewport(0, 0, frameBufferWidth, frameBufferHeight);
        configuration->windowHeight = frameBufferHeight;
        configuration->windowWidth = frameBufferWidth;

        glfwSwapInterval(0);

        // ----------------------
        // Init imgui
        // Setup Dear ImGui context
        debug_inf = new DebugInfos(window, *game);


        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        // glEnable(GL_BLEND);
        // glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);

        Shader mainShader("Sources/Shaders/textured_cube_vertex.glsl",
                          "Sources/Shaders/textured_cube_fragment.glsl");

        // Game variables
        // create vertices of what will beglUseProgram(shaderProgram); shown
        // 2. copy our vertices array in a buffer for OpenGL to useChun

        // ChunkRenderer cr;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // scene.world.load();
}

OpenGLWindow::~OpenGLWindow() {

        delete debug_inf;
        
        glfwTerminate();
}


bool OpenGLWindow::shouldContinue() {
        return !glfwWindowShouldClose(window);
}

void OpenGLWindow::beginFrameActions() {
        if (game->configuration.showDebugInfos) {
                debug_inf->initFrame();
        }
        
}

void OpenGLWindow::endFrameActions() {
        if (game->configuration.showDebugInfos) {
                debug_inf->drawFrame(*game);
        }
        glfwSwapBuffers(window);
}

void OpenGLWindow::mouseCallback(GLFWwindow *window, double xpos, double ypos) {
        ImGuiIO &io = ImGui::GetIO();
        if (!io.WantCaptureMouse) {
                Game *scene =
                    static_cast<Game *>(glfwGetWindowUserPointer(window));

                if (!scene->configuration.mouseFree) {
                        float xoffset = (float)xpos - scene->camera.lastX;
                        float yoffset =
                            scene->camera.lastY -
                            (float)ypos; // reversed since y-coordinates range
                                         // from bottom to top
                        scene->camera.lastX = (float)xpos;
                        scene->camera.lastY = (float)ypos;

                        const float sensitivity = 0.1f;
                        xoffset *= sensitivity;
                        yoffset *= sensitivity;

                        scene->camera.m_yaw += xoffset;
                        scene->camera.m_pitch += yoffset;

                        if (scene->camera.m_pitch > 89.0f)
                                scene->camera.m_pitch = 89.0f;
                        if (scene->camera.m_pitch < -89.0f)
                                scene->camera.m_pitch = -89.0f;

                        glm::vec3 direction;
                        direction.x = cos(glm::radians(scene->camera.m_yaw)) *
                                      cos(glm::radians(scene->camera.m_pitch));
                        direction.y = sin(glm::radians(scene->camera.m_pitch));
                        direction.z = sin(glm::radians(scene->camera.m_yaw)) *
                                      cos(glm::radians(scene->camera.m_pitch));
                        scene->camera.m_frontPos = glm::normalize(direction);
                        scene->camera.update();
                }
        }
}

void OpenGLWindow::framebufferSizeCallback(GLFWwindow *window, int width, int height) {
        Game *s = (Game *)glfwGetWindowUserPointer(window);
        s->configuration.windowHeight = height;
        s->configuration.windowWidth = width;
        s->camera.updateProjViewMatrix();
        glViewport(0, 0, width, height);
}

void OpenGLWindow::errorCallback(int error, const char *description) {
        std::cerr << "Erreur GLFW (" << error << "): " << description
                  << std::endl;
}

void OpenGLWindow::processInput() {
        static float currentFrame, deltaTime, lastFrame;
        currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        const float camera_speed = game->configuration.camSpeedFactor * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                game->camera.translate_forward(camera_speed);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                game->camera.translate_left(camera_speed);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                game->camera.translate_forward(-camera_speed);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                game->camera.translate_right(camera_speed);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
                game->camera.translate(camera_speed * glm::vec3(0., -1., 0.));
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
                game->camera.translate(camera_speed * glm::vec3(0., 1., 0.));
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
                game->configuration.camSpeedFactor = 50;
        } else {
                game->configuration.camSpeedFactor = 5;
        }
}

void OpenGLWindow::keyCallback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
        UNUSED(mods);
        UNUSED(scancode);
        Game *sc_ptr = (Game *)glfwGetWindowUserPointer(window);
        if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
                // to be fixed : check opengl polygone mode using some sort of
                // app variable that has been linked to the gl window
                GLint polyg_mode;
                glGetIntegerv(GL_POLYGON_MODE, &polyg_mode);

                if (polyg_mode == GL_LINE) {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                } else {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                }
        }
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                sc_ptr->configuration.mouseFree = !sc_ptr->configuration.mouseFree;
                if (sc_ptr->configuration.mouseFree) {
                        glfwSetInputMode(window, GLFW_CURSOR,
                                         GLFW_CURSOR_NORMAL);
                } else {
                        glfwSetInputMode(window, GLFW_CURSOR,
                                         GLFW_CURSOR_DISABLED);
                }
        }
        if (key == GLFW_KEY_F3 && action == GLFW_PRESS) {
                sc_ptr->configuration.showDebugInfos =
                    !sc_ptr->configuration.showDebugInfos;
                spdlog::debug("Show or hide now debug infos");
        }
}

void OpenGLWindow::mouseButtonCallback(GLFWwindow *window, int button, int action,
                           int mods) {
        UNUSED(mods);
        ImGuiIO &io = ImGui::GetIO();
        if (!io.WantCaptureMouse) {
                Game *sc_ptr = (Game *)glfwGetWindowUserPointer(window);
                if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
                        if (sc_ptr->configuration.mouseFree) {
                                sc_ptr->configuration.mouseFree =
                                    !sc_ptr->configuration.mouseFree;
                                glfwSetInputMode(window, GLFW_CURSOR,
                                                 GLFW_CURSOR_DISABLED);
                        }
                }
        }
}
