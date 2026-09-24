#ifndef WORLD_H
#define WORLD_H

#include "World/LockableChunk.h"
#include "WorldQueueOperation.h"
#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <unordered_map>

// in order to have the ability to hash the type for unordered map
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <mutex>

#include "CubeTexture.h"


class Camera;
class Chunk;
class ChunkRenderer;
class DebugInfos;
class WorldQueueOperation;
class WorldParallelOperationsManager;
class LockableChunk;



struct ChunkToLoadOnGPUBundle {
  Chunk * c;
  
};


class World {
      public:
        World();
        World(const World &w);
        ~World();


        CubeAtlas getBlockAt(glm::ivec3 pos);
        void setBlockAt(glm::ivec3 pos, CubeAtlas cube);

        std::optional<LockableChunk> getAbsoluteAt(glm::ivec2 abs_pos);

        void tagWithReason(glm::ivec2 abs_pos, ReasonToKeep r);
        void cleanAllChunksNoReasonToKeep();
        void IdontNeedAnymore(glm::ivec2 abs_pos, ReasonToKeep r);

      protected:

        std::unordered_map<glm::ivec2, LockableChunk> chunks;

        std::mutex chunks_lock;






};

#endif
