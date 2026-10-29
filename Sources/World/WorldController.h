#ifndef WORLD_CONTROLLER_H
#define WORLD_CONTROLLER_H

#include "Mesh.h"
#include <glm/glm.hpp>
#include <queue>
#include "Mathematics/AABB.h"
#include <mutex>
#include "Chunk.h"
#include "World/WorldParallelOperationsManager.h"
#include <memory>

struct MeshToLoadBundle {
  Mesh * ptr; // to be deleted when no more in use
  glm::ivec2 abs_pos;
  AABB bounding_box;
  
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

        int chunkWidth;

        Game *game;

        void moveWorld(glm::ivec2 new_player_pos);
        void loadOneMeshOnGpu();
};

#endif
