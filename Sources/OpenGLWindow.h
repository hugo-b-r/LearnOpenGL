#pragma once

#ifndef OPENGL_WINDOW_H
#define OPENGL_WINDOW_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>

// #include "Scene.h"
#include "Configuration.h"

class Game;




class OpenGLWindow {
      public:
        OpenGLWindow(Configuration* configuration);
        ~OpenGLWindow();

      protected:
        static void mouseCallback(GLFWwindow *window, double xpos, double ypos);
        static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
        static void errorCallback(int error, const char *description);
        static void processInput(GLFWwindow *window, Game &s);
        static void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                         int mods);
        static void mouseButtonCallback(GLFWwindow *window, int button, int action,
                                 int mods);


        GLFWwindow * window;
};

#endif
