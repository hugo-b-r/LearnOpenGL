#pragma once
#ifndef SCENE_H
#define SCENE_H

#include "Configuration.h"
#include "Camera.h"

#include "OpenGLWindow.h"
#include "World/World.h"

class Game {
public:
  Game(Configuration &c);

  // avec une camera précédement initialisée avec la configuration
  // Game(Configuration &c, Camera &cam);

  // avec une camera précédement initialisée avec la configuration
  // Game(Configuration &c, Camera &cam, World &world);

  Game();

  // Camera &getCamera();
  // Configuration &getConfig();
  // World &getWorld();

  void updateCollisionWithTerrainCheck(bool checkColTerrain);
  Configuration configuration;
  OpenGLWindow window;
  Camera camera;
  ChunkRenderer renderer;
  World world;

private:
};

#endif
