#ifndef OPENGL_WINDOW_H
#define OPENGL_WINDOW_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>

#include "Configuration.h"

class Game;
class DebugInfos;



/// Reprensents a window with an opengl context
///
/// It is using GLFW under the hood.
class OpenGLWindow {
      public:
        /// Default constructor
        OpenGLWindow(Configuration* configuration, Game * game);
        /// Default destructor
        ~OpenGLWindow();

        /// Whether we should continue or the windows has been asked to be closed
        bool shouldContinue();
        /// Handle all inputs from the window
        void processInput();
        /// Some actions needed at the beginning of the window
        void beginFrameActions();
        /// Some actions needed at the end of the window
        void endFrameActions();

      protected:
        /// callback for mouse events
        static void mouseCallback(GLFWwindow *window, double xpos, double ypos);
        /// callback for window resize events
        static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
        /// callback for errors that could happen
        static void errorCallback(int error, const char *description);
        /// callback for key sent
        static void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                         int mods);
        /// callback for mouse buttons clicks
        static void mouseButtonCallback(GLFWwindow *window, int button, int action,
                                 int mods);


        /// pointer to the GLFW window underneath
        GLFWwindow * window;
        /// debug infos windowing 
        DebugInfos * debug_inf;
        /// Pointer to the maing game
        Game * game;
};

#endif
