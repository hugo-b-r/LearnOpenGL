#include "Chunk.h"
#include "CubeTexture.h"
#include "World/ChunkMeshCreator.h"
#include "utils.h"

#include <spdlog/spdlog.h>

Chunk::Chunk()
    : blocks(CHUNK_HEIGHT * CHUNK_LENGTH * CHUNK_WIDTH, CubeAtlas::Air),
      chunkPos(glm::ivec2(0, 0)),
      m_modelMat(1.f), m_viewMat(1.f) {}

Chunk::Chunk(glm::ivec2 pos)
    : blocks(CHUNK_HEIGHT * CHUNK_LENGTH * CHUNK_WIDTH, CubeAtlas::Air),
      chunkPos(pos), m_modelMat(1.f),
      m_viewMat(1.f)  {}

Chunk::Chunk(int x, int y)
    : blocks(CHUNK_HEIGHT * CHUNK_LENGTH * CHUNK_WIDTH, CubeAtlas::Air),

      chunkPos(glm::ivec2(x, y)), m_modelMat(1.f), m_viewMat(1.f) {}

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


void Chunk::setBlockAt(glm::ivec3 block_pos, CubeAtlas ca) {
        return setBlockAt(block_pos.x, block_pos.y, block_pos.z, ca);
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

void Chunk::createBoundingBox() {
        glm::vec3 pos = fromChunkToBlockCoordinates(chunkPos, 0);
        m_boundingBox = {pos, pos + glm::vec3(CHUNK_WIDTH - 1, CHUNK_HEIGHT - 1,
                                              CHUNK_LENGTH - 1)};
}

AABB &Chunk::getBoundingBox() { return m_boundingBox; }


bool Chunk::containAbsPos(glm::ivec3 block_pos) {
        return fromBlockToChunkCoordinates(block_pos) == chunkPos;
}
bool Chunk::containRelPos(glm::ivec3 block_pos) {
        int x = block_pos.x;
        int y = block_pos.y;
        int z = block_pos.z;
        return x >= 0 && x < CHUNK_WIDTH && y >= 0 && y < CHUNK_HEIGHT && z >= 0 &&
            z < CHUNK_LENGTH;
}
