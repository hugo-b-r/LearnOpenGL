
#include <cstdlib>
#include <glad/glad.h>
#include "Scene.h"
#include "Renderer/ChunkRenderer.h"
#include "DebugInfos.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>



#include "Configuration.h"
#include "Shader.h"

#include <spdlog/spdlog.h>
// #define STB_IMAGE_IMPLEMENTATION
#include "../deps/imgui/imgui.h"
#include "../deps/stb_image.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "utils.h"

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  ImGuiIO &io = ImGui::GetIO();
  if (!io.WantCaptureMouse) {
    Scene *scene = static_cast<Scene *>(glfwGetWindowUserPointer(window));

    if (!scene->getConfig().mouseFree) {
      float xoffset = (float)xpos - scene->getCamera().lastX;
      float yoffset =
          scene->getCamera().lastY -
          (float)ypos; // reversed since y-coordinates range from bottom to top
      scene->getCamera().lastX = (float) xpos;
      scene->getCamera().lastY = (float) ypos;

      const float sensitivity = 0.1f;
      xoffset *= sensitivity;
      yoffset *= sensitivity;

      scene->getCamera().m_yaw += xoffset;
      scene->getCamera().m_pitch += yoffset;

      if (scene->getCamera().m_pitch > 89.0f)
        scene->getCamera().m_pitch = 89.0f;
      if (scene->getCamera().m_pitch < -89.0f)
        scene->getCamera().m_pitch = -89.0f;

      glm::vec3 direction;
      direction.x = cos(glm::radians(scene->getCamera().m_yaw)) *
                    cos(glm::radians(scene->getCamera().m_pitch));
      direction.y = sin(glm::radians(scene->getCamera().m_pitch));
      direction.z = sin(glm::radians(scene->getCamera().m_yaw)) *
                    cos(glm::radians(scene->getCamera().m_pitch));
      scene->getCamera().m_frontPos = glm::normalize(direction);
      scene->getCamera().update();
    }
  }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  Scene *s = (Scene *)glfwGetWindowUserPointer(window);
  s->getConfig().windowHeight = height;
  s->getConfig().windowWidth = width;
  s->getCamera().updateProjViewMatrix();
  glViewport(0, 0, width, height);
}

void error_callback(int error, const char *description) {
  std::cerr << "Erreur GLFW (" << error << "): " << description << std::endl;
}

void processInput(GLFWwindow *window, Scene &s) {
  static float currentFrame, deltaTime, lastFrame;
  currentFrame = (float) glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;

  const float camera_speed = s.getConfig().camSpeedFactor * deltaTime;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    s.getCamera().translate_forward(camera_speed);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    s.getCamera().translate_left(camera_speed);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    s.getCamera().translate_forward(-camera_speed);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    s.getCamera().translate_right(camera_speed);
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    s.getCamera().translate(camera_speed * glm::vec3(0., -1., 0.));
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    s.getCamera().translate(camera_speed * glm::vec3(0., 1., 0.));
  }
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
    s.getConfig().camSpeedFactor = 50;
  } else {
      s.getConfig().camSpeedFactor = 5;
  }
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
    UNUSED(mods);
    UNUSED(scancode);
  Scene *sc_ptr = (Scene *)glfwGetWindowUserPointer(window);
  if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
    // to be fixed : check opengl polygone mode using some sort of app
    // variable that has been linked to the gl window
    GLint polyg_mode;
    glGetIntegerv(GL_POLYGON_MODE, &polyg_mode);

    if (polyg_mode == GL_LINE) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } else {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
  }
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    sc_ptr->getConfig().mouseFree = !sc_ptr->getConfig().mouseFree;
    if (sc_ptr->getConfig().mouseFree) {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
  }
  if (key == GLFW_KEY_F3 && action == GLFW_PRESS) {
    sc_ptr->getConfig().showDebugInfos = !sc_ptr->getConfig().showDebugInfos;
    spdlog::debug("Show or hide now debug infos");
  }
}

void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods) {
  UNUSED(mods);
  ImGuiIO &io = ImGui::GetIO();
  if (!io.WantCaptureMouse) {
    Scene *sc_ptr = (Scene *)glfwGetWindowUserPointer(window);
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
      if (sc_ptr->getConfig().mouseFree) {
        sc_ptr->getConfig().mouseFree = !sc_ptr->getConfig().mouseFree;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      }
    }
  }
}
int main() {

  spdlog::set_level(spdlog::level::debug);
  Scene scene;

  // ----------------------
  // Init glfw

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  spdlog::info("GLFW has been initialized...");

  glfwSetErrorCallback(error_callback);

  GLFWwindow *window = glfwCreateWindow(scene.getConfig().windowWidth, scene.getConfig().windowHeight,
                                        "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    spdlog::error("Failed to create GLFW window");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    spdlog::error("Failed to initialize GLAD");
    return -1;
  }
  spdlog::info("Window has been created");
  glfwSetWindowUserPointer(window, &scene);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  glfwSetKeyCallback(window, key_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  int frameBufferWidth, frameBufferHeight;
  glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
  glViewport(0, 0, frameBufferWidth, frameBufferHeight);
  scene.getConfig().windowHeight = frameBufferHeight;
  scene.getConfig().windowWidth = frameBufferWidth;

  glfwSwapInterval(0);

  DebugInfos debug_inf(window);

  // ----------------------
  // Init imgui
  // Setup Dear ImGui context

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  Shader mainShader("Sources/Shaders/textured_cube_vertex.glsl",
                    "Sources/Shaders/textured_cube_fragment.glsl");

  // Scene variables
  // create vertices of what will beglUseProgram(shaderProgram); shown
  // 2. copy our vertices array in a buffer for OpenGL to useChun

  ChunkRenderer cr;
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    if (scene.getConfig().showDebugInfos) {
      debug_inf.initFrame();
    }
    processInput(window, scene);

    glClearColor(0.63f, 0.87f, 1.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    scene.getWorld().render(scene.getCamera(), cr, debug_inf);

    if (scene.getConfig().showDebugInfos) {
      debug_inf.drawFrame(scene);
    }

    glfwSwapBuffers(window);
  }

  // needed to delete before glfw terminate
  debug_inf.terminate();
  glfwTerminate();
  return EXIT_SUCCESS;
}
