#include "World.h"
#include "CubeTexture.h"
#include "Renderer/ChunkRenderer.h"
#include "World/LockableChunk.h"
#include "World/WorldParallelOperationsManager.h"
#include "WorldQueueOperation.h"
#include "utils.h"

#include <glm/glm.hpp>
#include <optional>
#include <spdlog/spdlog.h>

const glm::mat4 view_mat(1.0f);

World::World() {
        // chunkWidth include a border of chunks needed to build the other
        // chunks so that explains : (2 * (cam.getRenderDistance() + 1) + 1)
        // chunks.reserve(chunksNb);
}

World::World(const World &w) : chunks(w.chunks) {
}

World::~World() {}



std::optional<LockableChunk> World::getAbsoluteAt(glm::ivec2 abs_pos) {
        if (chunks.contains(abs_pos)) {
                return chunks.at(abs_pos);
        } else {
                return std::nullopt;
        }
}

CubeAtlas World::getBlockAt(glm::ivec3 pos) {
        glm::ivec2 chunkCoord = fromBlockToChunkCoordinates(pos);
        auto l_chunk_res = getAbsoluteAt(chunkCoord);
        if (l_chunk_res.has_value()) {
                auto blockInChunkPos = fromGlobalToInChunkCoordinates(pos);
                int a = blockInChunkPos.x;
                int b = blockInChunkPos.y;
                int c = blockInChunkPos.z;
                Chunk *chunk_ptr = l_chunk_res.value().lock();
                CubeAtlas res = chunk_ptr->getBlockAt(a, b, c);
                l_chunk_res.value().unlock();
                return res;
        } else {
                return CubeAtlas::Air;
        }
}


void World::setBlockAt(glm::ivec3 pos, CubeAtlas value) {
        glm::ivec2 chunkCoord = fromBlockToChunkCoordinates(pos);
        auto l_chunk_res = getAbsoluteAt(chunkCoord);
        if (l_chunk_res.has_value()) {
                auto blockInChunkPos = fromGlobalToInChunkCoordinates(pos);
                int a = blockInChunkPos.x;
                int b = blockInChunkPos.y;
                int c = blockInChunkPos.z;
                Chunk *chunk_ptr = l_chunk_res.value().lock();
                chunk_ptr->setBlockAt(a, b, c, value);
                l_chunk_res.value().unlock();
        }
}



void World::IdontNeedAnymore(glm::ivec2 abs_pos, ReasonToKeep r) {
        auto lc_res = getAbsoluteAt(abs_pos);
        if (lc_res.has_value()) {
                lc_res.value().removeReason(r);
        }
}


void World::tagWithReason(glm::ivec2 abs_pos, ReasonToKeep r) {
        auto lc_res = getAbsoluteAt(abs_pos);
        if (lc_res.has_value()) {
                lc_res.value().addReason(r);
        }
}

void World::cleanAllChunksNoReasonToKeep() {
        for (auto it = chunks.begin(); it != chunks.end(); ++it) {
                it->second.lock();
                if (!it->second.anyReasonToKeep()) {
                        chunks.erase(it);
                }
        }
}

