#include "Scene.h"
#include "Camera.h"
#include "Configuration.h"
#include "Renderer/ChunkRenderer.h"
#include "World/World.h"

// creating a game should be straithforward
Game::Game(Configuration &c) : configuration(c), window(&c), camera(*this), renderer(), world(camera, renderer) {}


Game::Game() : configuration(), window(&configuration), camera(*this), renderer(), world(camera, renderer) {}



// Camera &Game::getCamera() { return m_cam; }
// Configuration &Game::getConfig() { return m_config; }

// World &Game::getWorld() { return m_world; }

/// Updates the camera collision detection and the configuration collision
/// detection
void Game::updateCollisionWithTerrainCheck(bool checkColTerrain) {

        configuration.setCheckCollisions(checkColTerrain);
        camera.updateTranslateFunc(checkColTerrain);
}
