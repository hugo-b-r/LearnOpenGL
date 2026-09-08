#ifndef DEBUG_INFOS
#define DEBUG_INFOS

#include "Scene.h"
#include <vector>

#include <GLFW/glfw3.h>

class Game;

const int NB_VALUES_FRAMERATE = 100;
const double FRAMERATE_TIME_INTERVAL = 0.1;
class DebugInfos {
    public:
        DebugInfos(GLFWwindow * window, Game& s);
        ~DebugInfos();

        // after glfwpollevents but before gl draw
        void initFrame();
        // should be used after opengl draw
        void drawFrame(Game& scene);
        // should be called before glfwTerminate
        void terminate();
        void updateDrawnChunks(int nb);
    private:
        float lastFrameTime;
        float framerate;
        std::vector<float> framerates_values;
        std::vector<float> framerates_times;
        double time_last_framerate;
        bool show_fr_plot;
        int drawnChunks;

        bool checkColTerrain;
        bool oldCheckColTerrain;
        Game &m_scene;
};

#endif
