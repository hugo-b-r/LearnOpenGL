#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"
#include "Configuration.h"

#include "OpenGLWindow.h"
#include "Renderer/ChunkRenderer.h"
#include "World/World.h"
#include "World/WorldController.h"
#include "World/WorldParallelOperationsManager.h"

/// This class handles the main logic of the game. It orchestrates the main
/// components

class Game {
      public:
        /// The expected constructor. You need a configuration to tell it what to do.
        /// \param c is the configuration.
        Game(Configuration &c);

        Game();

        /// The mainloop of the game, keeps executing until window asked to
        /// close
        void mainLoop();

        /// Do a check with terrain for collision
        /// \param checkColTerrain whether to check collisions with terrain is
        /// enabled or not
        void updateCollisionWithTerrainCheck(bool checkColTerrain);
        /// The configuration of the game (general parameters)
        Configuration configuration;
        /// The window in which we render things
        OpenGLWindow window;
        /// Where the player sees the world
        Camera camera;
        /// A renderer for chunks, where mesh are stored
        ChunkRenderer renderer;
        /// Where world data is stored
        World world;
        /// Handles the world generation, deletion...
        WorldController world_controller;
        /// Thread pool controller, assign tasks, execute them in parallel pool
        WorldParallelOperationsManager *thread_pool_manager;

      private:
};

#endif
