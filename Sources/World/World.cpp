#include "World.h"
#include "CubeTexture.h"
#include "Renderer/ChunkRenderer.h"
#include "World/ChunkMeshCreator.h"
#include "World/LockableChunk.h"
#include "World/WorldGenerator.h"
#include "World/WorldParallelOperationsManager.h"
#include "WorldQueueOperation.h"
#include "utils.h"

#include <glm/glm.hpp>
#include <memory>
#include <optional>
#include <spdlog/spdlog.h>
#include <stdexcept>

const glm::mat4 view_mat(1.0f);

World::World() {
        // chunkWidth include a border of chunks needed to build the other
        // chunks so that explains : (2 * (cam.getRenderDistance() + 1) + 1)
        // chunks.reserve(chunksNb);
}

World::World(const World &w) : chunks(w.chunks) {}

World::~World() {}


std::optional<ReadSafeChunk> World::getAbsoluteChunkAt(glm::ivec2 abs_pos) {
        if (chunks.contains(abs_pos)) {
                return chunks.at(abs_pos);
        } else {
                return std::nullopt;
        }
}

std::optional<CubeAtlas> World::getBlockAt(glm::ivec3 pos) {
        glm::ivec2 chunkCoord = fromBlockToChunkCoordinates(pos);
        auto l_chunk_res = getAbsoluteChunkAt(chunkCoord);
        if (l_chunk_res.has_value()) {
                auto blockInChunkPos = fromGlobalToInChunkCoordinates(pos);
                int a = blockInChunkPos.x;
                int b = blockInChunkPos.y;
                int c = blockInChunkPos.z;
                CubeAtlas res = l_chunk_res.value().get()->getBlockAt(a, b, c);
                return res;
        } else {
                return std::nullopt;
        }
}

// void World::setBlockAt(glm::ivec3 pos, CubeAtlas value) {
//         // glm::ivec2 chunkCoord = fromBlockToChunkCoordinates(pos);
//         // auto l_chunk_res = getAbsoluteAt(chunkCoord);
//         // if (l_chunk_res.has_value()) {
//         //         auto blockInChunkPos =
//         fromGlobalToInChunkCoordinates(pos);
//         //         int a = blockInChunkPos.x;
//         //         int b = blockInChunkPos.y;
//         //         int c = blockInChunkPos.z;
//         //         chunk_ptr->setBlockAt(a, b, c, value);
//         //         l_chunk_res.value().unlock();
//         // }
// }

// void World::IdontNeedAnymore(glm::ivec2 abs_pos, ReasonToKeep r) {
//         auto reasons = reasons_manager.at(abs_pos);
//         reasons.removeReason(r);

//         // to delete code
//         throw std::runtime_error("implement queue for removing chunks");
//         // if (!reasons.anyReasonToKeep()) {
//         //         Chunk * c = lc_res.value().lock();
//         //         chunks.erase(c->getChunkPos());
//         // }
// }

// void World::tagWithReason(glm::ivec2 abs_pos, ReasonToKeep r) {
//         auto lc_res = reasons_manager.at(abs_pos);
//         lc_res.addReason(r);
// }

void World::cleanAllChunksNoReasonToKeep(glm::ivec2 new_player_pos, int manhattan_radius_limit) {

        // for (auto it = chunks.begin(); it != chunks.end(); ++it) {
        //         if (!it->second.anyReasonToKeep()) {
        //                 chunks.erase(it);
        //         }
        // }
}

//
void World::loadChunk(glm::ivec2 chunk_pos) {
        // for now, the only source for a chunk is the chunk builder
        auto c = std::make_shared<Chunk>(Chunk(chunk_pos));

        WorldGenerator::buildChunk(*(chunks[chunk_pos].get()), *this);

        loadChunk(c);

}

void World::loadChunk(ReadSafeChunk chunk) {
        // for now, the only source for a chunk is the chunk builder
        chunk_map_locker.lock();
        chunks[chunk.get()->getChunkPos()] = chunk;
        chunk_map_locker.unlock();

}

void World::unloadChunk(glm::ivec2 chunk_pos) {
        chunks.erase(chunk_pos);
}


bool World::isChunkLoaded(glm::ivec2 chunk_pos) {
        return chunks.contains(chunk_pos);
}
