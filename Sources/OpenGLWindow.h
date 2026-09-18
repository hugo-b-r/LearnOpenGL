#ifndef OPENGL_WINDOW_H
#define OPENGL_WINDOW_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>

#include "World/World.h"
#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>

#include "Configuration.h"

class Game;
class DebugInfos;




class OpenGLWindow {
      public:
        OpenGLWindow(Configuration* configuration, Game * game);
        ~OpenGLWindow();

        bool shouldContinue();
        void processInput();
        void beginFrameActions();
        void endFrameActions();

      protected:
        static void mouseCallback(GLFWwindow *window, double xpos, double ypos);
        static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
        static void errorCallback(int error, const char *description);
        static void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                         int mods);
        static void mouseButtonCallback(GLFWwindow *window, int button, int action,
                                 int mods);


        GLFWwindow * window;
        DebugInfos * debug_inf;
        Game * game;
};

#endif
