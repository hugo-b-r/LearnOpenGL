#include "Chunk.h"
#include "CubeTexture.h"
#include "World/ChunkMeshCreator.h"
#include "World/WorldGenerator.h"
#include "utils.h"

#include <spdlog/spdlog.h>

Chunk::Chunk()
    : m_mesh({{}, {}, {}}),
      blocks(CHUNK_HEIGHT * CHUNK_LENGTH * CHUNK_WIDTH, CubeAtlas::Air),
      m_loadStatus(ChunkDataLoadStatus::None),
      m_meshBuildStatus(ChunkMeshBuildStatus::NotStarted),
      chunkPos(glm::ivec2(0, 0)), m_modelMat(1.f), m_viewMat(1.f),
      m_numberVertices(0) {}

Chunk::Chunk(glm::ivec2 pos)
    : m_mesh({{}, {}, {}}),
      blocks(CHUNK_HEIGHT * CHUNK_LENGTH * CHUNK_WIDTH, CubeAtlas::Air),
      m_loadStatus(ChunkDataLoadStatus::None),
      m_meshBuildStatus(ChunkMeshBuildStatus::NotStarted), chunkPos(pos),
      m_modelMat(1.f), m_viewMat(1.f), m_numberVertices(0) {}

Chunk::Chunk(int x, int y)
    : m_mesh({{}, {}, {}}),
      blocks(CHUNK_HEIGHT * CHUNK_LENGTH * CHUNK_WIDTH, CubeAtlas::Air),
      m_loadStatus(ChunkDataLoadStatus::None),
      m_meshBuildStatus(ChunkMeshBuildStatus::NotStarted),
      chunkPos(glm::ivec2(x, y)), m_modelMat(1.f), m_viewMat(1.f),
      m_numberVertices(0) {}

Chunk::~Chunk() {}

glm::ivec2 Chunk::getChunkPos() { return chunkPos; }

glm::ivec3 Chunk::getChunkPosCenter() {
        return fromChunkToBlockCoordinates(chunkPos, CHUNK_HEIGHT / 2) +
               glm::ivec3(CHUNK_WIDTH / 2, 0, CHUNK_LENGTH / 2);
}

glm::vec3 Chunk::getChunkPosCenter3() {
        return glm::vec3(chunkPos.x * CHUNK_WIDTH + CHUNK_WIDTH / 2,
                         CHUNK_HEIGHT / 2,
                         chunkPos.y * CHUNK_LENGTH + CHUNK_LENGTH / 2);
}

CubeAtlas Chunk::getBlockAt(int x, int y, int z) {
        CubeAtlas res = CubeAtlas::Air;
        if (x < CHUNK_WIDTH && x >= 0 && y < CHUNK_HEIGHT && y >= 0 &&
            z < CHUNK_LENGTH && z >= 0) {
                assert(x * CHUNK_HEIGHT * CHUNK_LENGTH + z * CHUNK_HEIGHT + y >=
                       0);
                assert(x * CHUNK_HEIGHT * CHUNK_LENGTH + z * CHUNK_HEIGHT + y <
                       CHUNK_HEIGHT * CHUNK_LENGTH * CHUNK_WIDTH);
                res = blocks[x * CHUNK_HEIGHT * CHUNK_LENGTH +
                             z * CHUNK_HEIGHT + y];
        }
        return res;
}

CubeAtlas Chunk::getBlockAt(glm::ivec3 pos) {
        CubeAtlas res = getBlockAt(pos.x, pos.y, pos.z);

        return res;
}

void Chunk::setBlockAt(int x, int y, int z, CubeAtlas ca) {
        if (x >= 0 && x < CHUNK_WIDTH && y >= 0 && y < CHUNK_HEIGHT && z >= 0 &&
            z < CHUNK_LENGTH) {
                assert(x * CHUNK_HEIGHT * CHUNK_LENGTH + z * CHUNK_HEIGHT + y >=
                       0);
                assert(x * CHUNK_HEIGHT * CHUNK_LENGTH + z * CHUNK_HEIGHT + y <
                       CHUNK_HEIGHT * CHUNK_LENGTH * CHUNK_WIDTH);
                blocks[x * CHUNK_HEIGHT * CHUNK_LENGTH + z * CHUNK_HEIGHT + y] =
                    ca;
        }
}

void Chunk::load(World *w) {
        // generate the chunk if not loaded
        spdlog::debug("Loading chunk at pos : {}, {}", chunkPos.x, chunkPos.y);
        if (m_loadStatus != ChunkDataLoadStatus::FinishedLoaded) {
                WorldGenerator::buildChunk(*this, *w);
        }
        createBoundingBox();
        m_loadStatus = ChunkDataLoadStatus::FinishedLoaded;
        spdlog::debug("Finished loading chunk at pos : {}, {}", chunkPos.x,
                      chunkPos.y);
}

bool Chunk::isLoaded() {
        return m_loadStatus == ChunkDataLoadStatus::FinishedLoaded;
}

void Chunk::createMesh(World *w, CubeTexture &ct) {
        spdlog::debug("Creating mesh for chunk : {}, {}", chunkPos.x,
                      chunkPos.y);

        ChunkMeshCreator builder(*this, ct.FactorHeight(), *w, &m_mesh);
        builder.buildMesh();
        m_meshBuildStatus = ChunkMeshBuildStatus::DoneNotLoaded;
        spdlog::debug(" End creating mesh for chunk : {}, {}", chunkPos.x,
                      chunkPos.y);
}

bool Chunk::hasMesh() {
        return m_meshBuildStatus == ChunkMeshBuildStatus::DoneNotLoaded ||
               m_meshBuildStatus == ChunkMeshBuildStatus::DoneLoaded;
}

// void Chunk::loadMeshOnGPU() {
//         spdlog::debug("Loading mesh for chunk : {}, {}", chunkPos.x,
//                       chunkPos.y);
//         m_model.loadData(m_mesh, {3, 2, 3});
//         m_meshBuildStatus = ChunkMeshBuildStatus::DoneLoaded;
//         spdlog::debug("Finished loading mesh for chunk : {}, {}", chunkPos.x,
//                       chunkPos.y);
// }

// bool Chunk::hasOnGPUMesh() {
//         return m_meshBuildStatus == ChunkMeshBuildStatus::DoneLoaded;
// }

// void Chunk::bindModel() { m_model.bindVAO(); }

// void Chunk::draw(Camera &cam, ChunkRenderer &renderer) {
//         renderer.render(*this, cam, m_mesh.vertexPositions.size());
// }

void Chunk::createBoundingBox() {
        glm::vec3 pos = fromChunkToBlockCoordinates(chunkPos, 0);
        m_boundingBox = {pos, pos + glm::vec3(CHUNK_WIDTH - 1, CHUNK_HEIGHT - 1,
                                              CHUNK_LENGTH - 1)};
}

AABB &Chunk::getBoundingBox() { return m_boundingBox; }
