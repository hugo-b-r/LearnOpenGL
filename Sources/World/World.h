#ifndef WORLD_H
#define WORLD_H


#include <glm/glm.hpp>
#include "Camera.h"
#include "CubeTexture.h"
#include "Renderer/ChunkRenderer.h"
#include "World/WorldRepository.h"


class World {
public:
  World();

  void render(Camera& cam, ChunkRenderer& renderer);

  CubeAtlas& blockAt(glm::ivec3 pos);
  void playerLeftClicked(Camera& cam);
  void playerRightClicked(Camera& cam);

protected:
    WorldRepository m_worldRepo;

    void update(Camera& cam);
};

#endif
