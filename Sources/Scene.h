#ifndef SCENE_H
#define SCENE_H

#include "Configuration.h"
#include "Camera.h"
#include "World/World.h"

class Scene {
public:
  Scene(Configuration &c);

  // avec une camera précédement initialisée avec la configuration
  Scene(Configuration &c, Camera &cam);

  // avec une camera précédement initialisée avec la configuration
  Scene(Configuration &c, Camera &cam, World &world);

  Scene();

  Camera &getCamera();
  Configuration &getConfig();
  World &getWorld();

  void updateCollisionWithTerrainCheck(bool checkColTerrain);

private:
  Configuration m_config;
  Camera m_cam;
  World m_world;
};

#endif
