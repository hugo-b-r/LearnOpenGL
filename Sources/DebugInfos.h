#ifndef DEBUG_INFOS
#define DEBUG_INFOS

#include "Scene.h"

#include <GLFW/glfw3.h>

class DebugInfos {
    public:
        DebugInfos(GLFWwindow * window);
        ~DebugInfos();

        // after glfwpollevents but before gl draw
        void initFrame();
        // should be used after opengl draw
        void drawFrame(Scene& scene);
};

#endif
