#include "Scene.h"
#include "Camera.h"
#include "Configuration.h"
#include "Renderer/ChunkRenderer.h"
#include "World/World.h"

// creating a game should be straithforward
Game::Game(Configuration &c) : configuration(c), window(&c, this), camera(*this), renderer(), world(camera, renderer) {}


Game::Game() : configuration(), window(&configuration, this), camera(*this), renderer(), world(camera, renderer) {}



// Camera &Game::getCamera() { return m_cam; }
// Configuration &Game::getConfig() { return m_config; }

// World &Game::getWorld() { return m_world; }

/// Updates the camera collision detection and the configuration collision
/// detection
//


void Game::mainLoop() {
        
        while (window.shouldContinue()) {
                glfwPollEvents();

                window.beginFrameActions();
                
                window.processInput();

                glClearColor(0.63f, 0.87f, 1.f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                world.render(camera, renderer);

                // if (scene.configuration.showDebugInfos) {
                //         debug_inf.drawFrame(scene);
                // }

                window.endFrameActions();
        }
}



void Game::updateCollisionWithTerrainCheck(bool checkColTerrain) {

        configuration.setCheckCollisions(checkColTerrain);
        camera.updateTranslateFunc(checkColTerrain);
}
