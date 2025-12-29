#include "Chunk.h"
#include "Camera.h"
#include "CubeTexture.h"
#include "World/ChunkMeshCreator.h"
#include "World/WorldGenerator.h"

#include <spdlog/spdlog.h>

Chunk::Chunk()
    : blocks(CHUNK_HEIGHT * CHUNK_LENGTH * CHUNK_WIDTH, CubeAtlas::Air),
      m_loadStatus(ChunkDataLoadStatus::None),
      m_meshBuildStatus(ChunkMeshBuildStatus::NotStarted), m_mesh({{}, {}, {}}),
      m_model(), chunkPos(glm::ivec2(0, 0)), m_modelMat(1.f), m_viewMat(1.f),
      m_numberVertices(0) {}

Chunk::Chunk(glm::ivec2 pos)
    : blocks(CHUNK_HEIGHT * CHUNK_LENGTH * CHUNK_WIDTH, CubeAtlas::Air),
      m_loadStatus(ChunkDataLoadStatus::None),
      m_meshBuildStatus(ChunkMeshBuildStatus::NotStarted), m_mesh({{}, {}, {}}),
      m_model(), chunkPos(pos), m_modelMat(1.f), m_viewMat(1.f),
      m_numberVertices(0) {}

glm::ivec2 Chunk::getChunkPos() { return chunkPos; }

glm::ivec2 Chunk::getChunkPosCenter() {
  return chunkPos + glm::ivec2(CHUNK_WIDTH / 2, CHUNK_LENGTH / 2);
}

glm::vec3 Chunk::getChunkPosCenter3() {
  return glm::vec3(chunkPos.x  * CHUNK_WIDTH + CHUNK_WIDTH/2, 0,
                   chunkPos.y * CHUNK_LENGTH + CHUNK_LENGTH/2);
}

CubeAtlas Chunk::getBlockAt(int x, int y, int z) {
  return blocks[x * CHUNK_HEIGHT * CHUNK_LENGTH + z * CHUNK_HEIGHT + y];
}

void Chunk::setBlockAt(int x, int y, int z, CubeAtlas ca) {
  blocks[x * CHUNK_HEIGHT * CHUNK_LENGTH + z * CHUNK_HEIGHT + y] = ca;
}

void Chunk::load() {
  // generate the chunk if not laoaded
  // for now: fun loading / generation function that does not take into account
  // therest of chunks
  if (m_loadStatus != ChunkDataLoadStatus::FinishedLoaded) {
      WorldGenerator::buildChunk(*this);
  }
  m_loadStatus = ChunkDataLoadStatus::FinishedLoaded;
}

bool Chunk::isLoaded() {
  return m_loadStatus == ChunkDataLoadStatus::FinishedLoaded;
}

void Chunk::createMesh(World& w) {
  CubeTexture ct("./Assets/texture_atlas.jpg");
  ChunkMeshCreator builder(*this, ct, w);

  m_numberVertices = builder.buildMesh();
  m_mesh = builder.getMesh();
  m_meshBuildStatus = ChunkMeshBuildStatus::DoneNotLoaded;
}

bool Chunk::hasMesh() {
  return m_meshBuildStatus == ChunkMeshBuildStatus::DoneNotLoaded ||
         m_meshBuildStatus == ChunkMeshBuildStatus::DoneLoaded;
}

void Chunk::loadMeshOnGPU() {
  m_model.loadData(m_mesh, {3, 2});
  m_meshBuildStatus = ChunkMeshBuildStatus::DoneLoaded;
}

bool Chunk::hasOnGPUMesh() {
  return m_meshBuildStatus == ChunkMeshBuildStatus::DoneLoaded;
}

void Chunk::bindModel() { m_model.bindVAO(); }

void Chunk::draw(Camera &cam, ChunkRenderer &renderer) {
  renderer.render(*this, cam, m_mesh.vertexPositions.size());
}
