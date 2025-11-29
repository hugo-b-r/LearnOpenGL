
#include "../deps/glad/include/glad/glad.h"


#include "Camera.h"
#include "Configuration.h"
#include "NonTexturedCubeRenderer.h"
#include "Shader.h"
#include "QuadRenderer.h"

#include "spdlog/spdlog.h"
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <iostream>


void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void error_callback(int error, const char* description)
{
    std::cerr << "Erreur GLFW (" << error << "): " << description << std::endl;
}

void processInput(GLFWwindow *window, float * rotation) {
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        *rotation = 1.;
    } else {
        *rotation = 0.;
    }
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  } else if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
      // to be fixed : check opengl polygone mode using some sort of app variable that has been linked to the gl window
    GLint polyg_mode;
    glGetIntegerv(GL_POLYGON_MODE, &polyg_mode);

    if (polyg_mode == GL_LINE) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
  }
}
int main() {

    Configuration conf;

  // ----------------------
  // Init glfw

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  spdlog::info("GLFW has been initialized...");

  glfwSetErrorCallback(error_callback);

  GLFWwindow *window =
      glfwCreateWindow(conf.windowWidth, conf.windowHeight, "LearnOpenGL", NULL, NULL);
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
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetKeyCallback(window, key_callback);
  int frameBufferWidth, frameBufferHeight;
  glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
  glViewport(0, 0, frameBufferWidth, frameBufferHeight);
  conf.windowHeight = frameBufferHeight;
  conf.windowWidth = frameBufferWidth;

  glEnable(GL_DEPTH_TEST);
  Shader mainShader("Sources/Shaders/quad_vertex.glsl", "Sources/Shaders/quad_fragment.glsl");
  Camera cam(conf);

  // Scene variables
  // create vertices of what will beglUseProgram(shaderProgram); shown
  // 2. copy our vertices array in a buffer for OpenGL to use
  QuadRenderer r;
  NonTexturedCubeRenderer cube_rend;
  float rotation = 0;

  while (!glfwWindowShouldClose(window)) {
    processInput(window, &rotation);


    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    cube_rend.render(mainShader, cam);
    //r.render(mainShader, cam);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
