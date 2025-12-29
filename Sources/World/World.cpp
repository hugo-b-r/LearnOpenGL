#include "World.h"
#include "Renderer/ChunkRenderer.h"
#include <unordered_set>

#include <spdlog/spdlog.h>
#include <glm/glm.hpp>

World::World() {}

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
  std::unordered_set<glm::ivec2> posToUnLoad = getLoadedChunkPos();
  // Draw each chunk
  for (int i = -cam.getRenderDistance()+camPos.x; i <= cam.getRenderDistance()+camPos.x; ++i)
    {
      for (int j = -cam.getRenderDistance()+camPos.y; j <= cam.getRenderDistance()+camPos.y; ++j) {
        // remove rendererd chunks
        auto tmpPos = glm::ivec2(i, j);
        if (posToUnLoad.contains(tmpPos))
          posToUnLoad.erase(tmpPos);
        if (!isLoadedAt(tmpPos)) {
            spdlog::debug("loading at : {}, {}", tmpPos.x, tmpPos.y);
            loadAt(tmpPos);
        }

        getAt(tmpPos).draw(cam, renderer);
      }
    }
  for (auto& posChunk: posToUnLoad) {
      unloadAt(posChunk);
      spdlog::debug("unloading at : {}, {}", posChunk.x, posChunk.y);

  }
}

void World::update(Camera &cam) {}



void World::loadAt(glm::ivec2 pos) {
    m_chunks.insert({pos, Chunk(pos)});
    m_chunks[pos].load();
    m_chunks[pos].createMesh();
    m_chunks[pos].loadMeshOnGPU();
}

void World::unloadAt(glm::ivec2 pos) {
    m_chunks.erase(pos);
}

bool World::isLoadedAt(glm::ivec2 pos) {
    if (!existsAt(pos)) {
        return false;
    }
    return m_chunks[pos].hasOnGPUMesh();
}

Chunk World::getAt(glm::ivec2 pos) {
    if (existsAt(pos)) {
        return m_chunks.at(pos);
    } else {
        return Chunk(pos);
    }
}

std::unordered_set<glm::ivec2> World::getLoadedChunkPos() {
    std::unordered_set<glm::ivec2> res;
    for (auto& c: m_chunks) {
        res.insert(std::get<0>(c));
    }
    return res;
}

std::unordered_map<glm::ivec2, Chunk> World::getChunks() {
    return m_chunks;
}

bool World::existsAt(glm::ivec2 pos) {
    return m_chunks.find(pos) != m_chunks.end();
}
