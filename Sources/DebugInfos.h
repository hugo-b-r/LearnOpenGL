#ifndef DEBUG_INFOS
#define DEBUG_INFOS

#include "Scene.h"
#include <vector>

#include <GLFW/glfw3.h>

class Game;

/// Number of values of framerate to retain in memory
const int NB_VALUES_FRAMERATE = 100;
/// The interval between framerates updates
const double FRAMERATE_TIME_INTERVAL = 0.1;

/// Main class containing debugging infos and window management system
class DebugInfos {
      public:
        /// Default constructor
        DebugInfos(GLFWwindow *window, Game &s);
        /// Default destructor
        ~DebugInfos();

        /// Init the window inside the app window.
        ///
        /// To be ran after glfwpollevents but before gl draw
        void initFrame();
        /// Effectively draws the debug infos window frame
        ///
        /// It should be used after opengl draw
        void drawFrame(Game &scene);
        /// Finalize the windo
        ///
        /// It should be called before glfwTerminate
        void terminate();
        /// Setter for the number of chunks drawn
        void updateDrawnChunks(int nb);

      private:
        /// Time duration from last frame
        float lastFrameTime;
        /// Current framerate
        ///
        /// That is an average of 1/frametime
        float framerate;
        /// The retained framerates values
        ///
        /// That is used to draw the graph of the framerete evolution in time.
        std::vector<float> framerates_values;

        /// The retained framerate times
        ///
        /// These are the times used in coordination with #framerates_values.
        /// That is used to draw the graph of the framerete evolution in time.
        std::vector<float> framerates_times;
        /// The time at which we computed last framerate.
        ///
        /// This is used to know how much time last fame took.
        double time_last_framerate;
        /// Whether to show a framerate plot of its evolution over time
        bool show_fr_plot;
        /// The number of drawnChunks
        int drawnChunks;

        /// whether we check collisions with terrain or not
        bool checkColTerrain;
        /// whether we checked collisions with terrain or not
        ///
        /// Used to know if there is an update or not, and therefore call the
        /// right function or not.
        bool oldCheckColTerrain;
        /// A reference to the main game
        Game &m_scene;
};

#endif
