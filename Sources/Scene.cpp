#include "Scene.h"
#include "Camera.h"
#include "Configuration.h"
#include "Renderer/ChunkRenderer.h"
#include "World/World.h"

#include "World/WorldController.h"
#include "World/WorldParallelOperationsManager.h"
#include "utils.h"

// creating a game should be straithforward
Game::Game(Configuration &c) : configuration(c), window(&c, this), camera(*this), renderer(), world(), world_controller(&world, &renderer, this, camera.getRenderDistance()) {
        thread_pool_manager = new WorldParallelOperationsManager(&world_controller);
        world_controller.initialLoad(thread_pool_manager);
}


Game::Game() : configuration(), window(&configuration, this), camera(*this), renderer(), world(), world_controller(&world, &renderer, this, camera.getRenderDistance()) {
        thread_pool_manager = new WorldParallelOperationsManager(&world_controller);
        world_controller.initialLoad(thread_pool_manager);
}



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

                renderer.render(camera);


                // world_controller.perFrameControl(fromFloatToChunkCoordinates(camera.m_frontPos));
                world_controller.perFrameControl(camera.chunkPosition());

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
