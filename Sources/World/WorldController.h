#ifndef WORLD_CONTROLLER_H
#define WORLD_CONTROLLER_H

#include "Mesh.h"
#include "utils.h"
// #include "gmock/gmock.h"
#include <glm/glm.hpp>
// #define GLM_ENABLE_EXPERIMENTAL
// #include <glm/gtx/hash.hpp>
#include <queue>
#include "Mathematics/AABB.h"
#include <mutex>
#include "Chunk.h"
#include "World/WorldParallelOperationsManager.h"
#include <memory>
#include <unordered_map>

struct MeshToLoadBundle {
  Mesh * ptr; // to be deleted when no more in use
  glm::ivec2 abs_pos;
  AABB bounding_box;
  
};

/// For controlling the properties of a chunk (whether it is loaded in the world, in the renderer....)
struct ChunkControlProperties {
  bool in_world = false;
  bool in_renderer = false;
};

class Game;
class World;
class ChunkRenderer;

class WorldController {
      public:
        WorldController(World *w, ChunkRenderer *renderer, Game *g, int render_distance);
        ~WorldController();

        World *world;
        ChunkRenderer *renderer;
        void perFrameControl(glm::ivec2 new_player_pos);
        void addMeshToLoadJob(glm::ivec2 abs_pos, Mesh * mesh, AABB bounding_box);

        void initialLoad(WorldParallelOperationsManager *thread_pool_manager);
        void addMeshAndGPULoadChunkJob(std::shared_ptr<Chunk>);

      protected:
        glm::ivec2 lastPlayerChunkPos;

        std::queue<MeshToLoadBundle> meshes_to_load;
        std::mutex meshes_to_load_lock;

        std::unordered_map<glm::ivec2, ChunkControlProperties, KeyFuncs, KeyFuncs> chunk_properties;
        std::mutex chunk_properties_lock;

        int chunkWidth;

        Game *game;

        void removeChunkHandleWorldAndGPU(glm::ivec2 chunk_pos);
        void moveWorld(glm::ivec2 new_player_pos);
        void loadOneMeshOnGpu();

        void removeChunkFromGPU(glm::ivec2 abs_pos);
        void removeChunkFromWorld(glm::ivec2 abs_pos);
};

#endif
