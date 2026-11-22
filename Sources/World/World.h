#ifndef WORLD_H
#define WORLD_H

#include "World/WorldParallelOperationsManager.h"
#include "WorldQueueOperation.h"
#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>

#include <utils.h>

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

/// Simple name for a reference hold to a chunk
using ReadSafeChunk = std::shared_ptr<Chunk>;

/// The main world holding informations about every part of the terrain.
class World {
      public:
        /// Default constructor
        World(WorldController& world_controller);
        /// Default copoy constructor
        World(const World &w);
        /// Default destructor
        ~World();

        /// Returns block at \a pos, looking for inside chunks
        ///
        /// Optionnal because can be not found
        std::optional<CubeAtlas> getBlockAt(glm::ivec3 pos);

        /// Returns a reference to a chunk
        std::optional<ReadSafeChunk> getAbsoluteChunkAt(glm::ivec2 abs_pos);

        /// Add a #Chunk to the world
        ///
        /// Actually add it to a queueu and load it at some point
        void loadChunk(ReadSafeChunk chunk);
        /// Tells whether a #Chunk is present in the world
        bool isChunkLoaded(glm::ivec2 chunk_pos);
        /// Remove a #Chunk to the world
        void unloadChunk(glm::ivec2 chunk_pos);

        /// Returns the neighbors or a chunk.
        ///
        /// All 4 if exists, eles nullopt. Used in meshing for example when you
        /// always need 4
        std::optional<NeighboringChunks> getChunkNeighbors(glm::ivec2 abs_pos);
        /// The map of chunks loaded. Use that with #chunk_map_locker
        std::unordered_map<glm::ivec2, ReadSafeChunk, KeyFuncs, KeyFuncs>
            chunks;
        /// The locker for #chunks
        std::mutex chunk_map_locker;

        /// Finds blocks to be loaded from #blocks_to_load, adds them to the \a
        /// chunk and finally remove them from the world
        void applyUnloadedBlockstoNewChunk(Chunk &chunk);

        /// Adds \a blocks to the list of blocks that needs to be loaded for other chunks
        void appendNewUnloadedBlocks(std::vector<std::pair<glm::ivec3, CubeAtlas>> blocks);

        /// Build a new chunk at \a pos adequate with the changes in cache in #blocks_to_load
        /// Optional because it can be non existing
        std::optional<std::shared_ptr<Chunk>> buildNextChunkFor(glm::ivec2 pos);

      protected:

        /// to allow the world to indirecly control itself
        WorldController& world_controller;
        /// Stores the blocks that can't be added to chunks that are not yet
        /// loaded but might be.
        std::unordered_map<glm::ivec2,
                           std::vector<std::pair<glm::ivec3, CubeAtlas>>,
                           KeyFuncs, KeyFuncs>
            blocks_to_load;

        /// Mutex for #blocks_to_load
        std::mutex blocks_to_load_locker;
};

#endif
