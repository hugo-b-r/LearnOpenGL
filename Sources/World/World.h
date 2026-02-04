#ifndef WORLD_H
#define WORLD_H

#include <glm/glm.hpp>
#include <unordered_map>
#include <unordered_set>
// in order to have the ability to hash the type for unordered map
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "Chunk.h"
#include "CubeTexture.h"
#include "Renderer/ChunkRenderer.h"

class Camera;
class Chunk;
class ChunkRenderer;
class DebugInfos;

class World {
public:
  World();

  void render(Camera &cam, ChunkRenderer &renderer, DebugInfos &inf);

  CubeAtlas getBlockAt(glm::ivec3 pos);
  void setBlockAt(glm::ivec3 pos, CubeAtlas cube);

  void playerLeftClicked(Camera &cam);
  void playerRightClicked(Camera &cam);

  void loadAt(glm::ivec2 pos);
  void unloadAt(glm::ivec2 pos);
  bool isLoadedAt(glm::ivec2 pos);
  void createAndLoadMeshAt(glm::ivec2 pos);
  bool isMeshLoadedAt(glm::ivec2 pos);

  Chunk getAt(glm::ivec2 pos);

  std::unordered_set<glm::ivec2> getLoadedChunkPos();

  std::unordered_map<glm::ivec2, Chunk> getChunks();

protected:
  void update();
  bool existsAt(glm::ivec2 pos);
  std::unordered_map<glm::ivec2, Chunk> m_chunks;
};

#endif
