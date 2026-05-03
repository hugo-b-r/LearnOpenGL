#ifndef WORLD_H
#define WORLD_H

#include <glm/glm.hpp>
#include <unordered_map>
#include <unordered_set>
#include <vector>
// in order to have the ability to hash the type for unordered map
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "Chunk.h"
#include "CubeTexture.h"
#include "Renderer/ChunkRenderer.h"

class Camera;
class Chunk;
class ChunkRenderer;
class DebugInfos;

class World {
      public:
        World(Camera &cam);
        ~World();

        // procedure to load the world correctly
        void load();
        void render(Camera &cam, ChunkRenderer &renderer, DebugInfos &inf);

        /// returns the block present at the coord (assuming they are absolute) if exists
        CubeAtlas getBlockAt(glm::ivec3 pos);
        void setBlockAt(glm::ivec3 pos, CubeAtlas cube);

        void playerLeftClicked(Camera &cam);
        void playerRightClicked(Camera &cam);

        bool isLoadedAt(glm::ivec2 pos);
        void createAndLoadMeshAt(glm::ivec2 pos);
        bool isMeshLoadedAt(glm::ivec2 pos);

        /// returns chunk at the position pos
        Chunk *getRelativeToPlayerAt(glm::ivec2 pos);
        /// returns chunk at the position pos
        Chunk *getRelativeToPlayerAt(int x, int y);

        /// returns the chunk present at a pos diff to player's pos
        Chunk *getAbsoluteAt(glm::ivec2 pos);
        /// returns the chunk present at a pos diff to player's pos
        Chunk *getAbsoluteAt(int x, int y);

        /// set the chunk present at a pos diff to player's pos
        void setRelativeToPlayerAt(glm::ivec2 pos, Chunk *c);
        /// set the chunk present at a pos diff to player's pos
        void setRelativeToPlayerAt(int x, int y, Chunk *c);

        std::unordered_set<glm::ivec2> getLoadedChunkPos();

      protected:


        // move all the map one Chunk in x + direction
        void moveChunkXP();
        // move all the map one Chunk in x - direction
        void moveChunkXM();
        // move all the map one Chunk in y + direction
        void moveChunkYP();
        // move all the map one Chunk in y - direction
        void moveChunkYM();

        /// tests wether the chunk exitsts at this position (absolute position)
        bool existsAt(glm::ivec2 pos);

        int chunkWidth;
        int chunksNb;

        std::vector<Chunk *> chunks;

        // the world is existing next to a camera
        Camera& camera;
        // the pos where the player was before
        glm::ivec2 lastChunkPlayerPos;

};

#endif
