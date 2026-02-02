#include "Camera.h"
#include "World.h"
#include "CubeTexture.h"
#include "Renderer/ChunkRenderer.h"
#include "World/Chunk.h"
#include "utils.h"
#include <unordered_set>
#include "DebugInfos.h"


#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

const glm::mat4 view_mat(1.0f);

World::World() {}

void World::render(Camera &cam, ChunkRenderer &renderer, DebugInfos& inf) {
  /*
   * Logic :
   * updates the world to unload unused chunks, load used chunks
   * for each chunk in the repository : draw it
   *
   */
  update();
  renderer.bindTextureAndShader();
  glm::ivec2 camPos = cam.chunkPosition();
  Frustum &f = cam.getFrustum();
  // loading terrain
  for (int i = -cam.getRenderDistance() - 1 + camPos.x;
       i <= cam.getRenderDistance() + 1 + camPos.x; ++i) {
    for (int j = -cam.getRenderDistance() - 1 + camPos.y;
         j <= cam.getRenderDistance() + 1 + camPos.y; ++j) {
      // remove rendererd chunks
      auto tmpPos = glm::ivec2(i, j);
      if (!isLoadedAt(tmpPos)) {
        spdlog::debug("loading at : {}, {}", tmpPos.x, tmpPos.y);
        loadAt(tmpPos);
      }
    }
  }

  //std::unordered_set<glm::ivec2> posToUnLoad = getLoadedChunkPos();
  // Draw each chunk

  int total_drawn = 0;
  renderer.getShader().setMat4("view", view_mat);
  renderer.getShader().setMat4("projection", cam.getProjectionViewMatrix());
  for (int i = -cam.getRenderDistance() + camPos.x;
       i <= cam.getRenderDistance() + camPos.x; ++i) {
    for (int j = -cam.getRenderDistance() + camPos.y;
         j <= cam.getRenderDistance() + camPos.y; ++j) {
      // remove rendererd chunks
      auto tmpPos = glm::ivec2(i, j);
      // if (posToUnLoad.contains(tmpPos))
      //   posToUnLoad.erase(tmpPos);
      if (!isMeshLoadedAt(tmpPos)) {
        createAndLoadMeshAt(tmpPos);
      }
      if (f.IsAabbVisible(getAt(tmpPos).getBoundingBox())) {
        getAt(tmpPos).draw(cam, renderer);
        ++total_drawn;
      }
    }
    // for now, we never unload as the data structure is not great for that
    //   }
    // for (auto& posChunk: posToUnLoad) {
    //     unloadAt(posChunk);
    //     spdlog::debug("unloading at : {}, {}", posChunk.x, posChunk.y);

    // }

     inf.updateDrawnChunks(total_drawn);

  }
}

CubeAtlas World::getBlockAt(glm::ivec3 pos) {
  auto chunkCoord = fromBlockToChunkCoordinates(pos);
  if (existsAt(chunkCoord)) {
    auto blockInChunkPos = fromGlobalToInChunkCoordinates(pos);
    int a = blockInChunkPos.x, b = blockInChunkPos.y, c = blockInChunkPos.z;
    return m_chunks[chunkCoord].getBlockAt(a, b, c);
  } else {
    return CubeAtlas::Air;
  }
}

void World::update() {
}

void World::loadAt(glm::ivec2 pos) {
  m_chunks.insert({pos, Chunk(pos)});
  m_chunks[pos].load();

}

void World::unloadAt(glm::ivec2 pos) { m_chunks.erase(pos); }

bool World::isLoadedAt(glm::ivec2 pos) {
  if (!existsAt(pos)) {
    return false;
  }
  return m_chunks[pos].isLoaded();
}

void World::createAndLoadMeshAt(glm::ivec2 pos) {
  if (existsAt(pos)) {
    m_chunks[pos].createMesh(*this);
    m_chunks[pos].loadMeshOnGPU();
  }
}

bool World::isMeshLoadedAt(glm::ivec2 pos) {
  if (existsAt(pos)) {
    return m_chunks[pos].hasOnGPUMesh();
  } else {
    return false;
  }
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
  for (auto &c : m_chunks) {
    res.insert(std::get<0>(c));
  }
  return res;
}

std::unordered_map<glm::ivec2, Chunk> World::getChunks() { return m_chunks; }

bool World::existsAt(glm::ivec2 pos) {
  return m_chunks.find(pos) != m_chunks.end();
}
