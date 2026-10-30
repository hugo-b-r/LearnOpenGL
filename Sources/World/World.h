#ifndef WORLD_H
#define WORLD_H

#include "WorldQueueOperation.h"
#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>

// in order to have the ability to hash the type for unordered map
// #define GLM_ENABLE_EXPERIMENTAL
// #include <glm/gtx/hash.hpp>
//
#include <utils.h>

#include "World/LockableChunk.h"
#include <mutex>
#include <optional>

#include "CubeTexture.h"

class Camera;
class Chunk;
class ChunkRenderer;
class DebugInfos;
class WorldQueueOperation;
class WorldParallelOperationsManager;
class RemoveSafeChunk;

using ReadSafeChunk = std::shared_ptr<Chunk>;

class World {
      public:
        World();
        World(const World &w);
        ~World();

        void initialLoad();

        std::optional<CubeAtlas> getBlockAt(glm::ivec3 pos);
        // void setBlockAt(glm::ivec3 pos, CubeAtlas cube);

        std::optional<ReadSafeChunk> getAbsoluteChunkAt(glm::ivec2 abs_pos);

        void tagWithReason(glm::ivec2 abs_pos, ReasonToKeep r);
        void cleanAllChunksNoReasonToKeep(glm::ivec2 new_player_pos,
                                          int manhattan_radius_limit);
        void IdontNeedAnymore(glm::ivec2 abs_pos, ReasonToKeep r);
        void loadChunk(ReadSafeChunk chunk);
        bool isChunkLoaded(glm::ivec2 chunk_pos);
        void unloadChunk(glm::ivec2 chunk_pos);

        std::optional<NeighboringChunks> getChunkNeighbors(glm::ivec2 abs_pos);
        std::unordered_map<glm::ivec2, ReadSafeChunk, KeyFuncs, KeyFuncs>
            chunks;
        std::mutex chunk_map_locker;

      protected:
        void loadChunk(glm::ivec2 chunk_pos);
        // std::unordered_map<glm::ivec2, ReasonToKeepManager> reasons_manager;

        std::mutex chunks_lock;
};

#endif
