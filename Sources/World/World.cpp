#include "World.h"
#include "Renderer/ChunkRenderer.h"
#include "World/WorldRepository.h"
#include <unordered_set>

#include <spdlog/spdlog.h>

World::World() : m_worldRepo() {}

void World::render(Camera &cam, ChunkRenderer& renderer) {
  /*
   * Logic :
   * updates the world to unload unused chunks, load used chunks
   * for each chunk in the repository : draw it
   *
   */
  // Update the world
  update(cam);

  glm::ivec2 camPos = cam.chunkPosition();
  std::unordered_set<glm::ivec2> posToUnLoad = m_worldRepo.getLoadedChunkPos();
  // Draw each chunk
  for (int i = -cam.getRenderDistance()+camPos.x; i <= cam.getRenderDistance()+camPos.x; ++i)
    {
      for (int j = -cam.getRenderDistance()+camPos.y; j <= cam.getRenderDistance()+camPos.y; ++j) {
        // remove rendererd chunks
        auto tmpPos = glm::ivec2(i, j);
        if (posToUnLoad.contains(tmpPos))
          posToUnLoad.erase(tmpPos);
        if (!m_worldRepo.isLoadedAt(tmpPos)) {
            spdlog::debug("loading at : {}, {}", tmpPos.x, tmpPos.y);
            m_worldRepo.loadAt(tmpPos);
        }

        m_worldRepo.getAt(tmpPos).draw(cam, renderer);
      }
    }
  for (auto& posChunk: posToUnLoad) {
      m_worldRepo.unloadAt(posChunk);
  }
}

void World::update(Camera &cam) {}
