#ifndef WORLD_CONTROLLER_H
#define WORLD_CONTROLLER_H

#include "Chunk.h"
#include "Mathematics/AABB.h"
#include "Mesh.h"
#include "World/WorldParallelOperationsManager.h"
#include "utils.h"
#include <glm/glm.hpp>
#include <memory>
#include <mutex>
#include <queue>
#include <unordered_map>

const int number_chunks_to_delete_at_a_time = 4;

/// Bundle to put inside the queue of waiting to be loaded chunks
struct MeshToLoadBundle {
        /// Pointer to the mesh, needs to be deleted after
        Mesh *ptr;
        /// Absolute position of the chunk concerned
        glm::ivec2 abs_pos;
        /// #AABB for the chunk
        AABB bounding_box;
};

/// Chunk presence in world or renderer
///
/// For controlling the properties of a chunk (whether it is loaded in the
/// world, in the renderer....)
struct ChunkControlProperties {
        /// Is the chunk present in the world ?
        bool in_world = false;
        /// Is the chunk present in the renderer ?
        bool in_renderer = false;
};

class Game;
class World;
class ChunkRenderer;

/// Controls the world loading, unloading, mesh move to renderer...
///
/// Also manages removing from renderer...
class WorldController {
      public:
        /// Base constructor
        WorldController(World *w, ChunkRenderer *renderer, Game *g,
                        int render_distance);

        /// Bas destructor
        ~WorldController();

        /// The managed world
        World *world;
        /// The renderer for the #world
        ChunkRenderer *renderer;
        /// This function must be called every frame as it does management
        void perFrameControl(glm::ivec2 new_player_pos);
        /// Add the mesh to load queue
        void addMeshToLoadJob(glm::ivec2 abs_pos, Mesh *mesh,
                              AABB bounding_box);

        /// Add the mesh to reload queue
        void addMeshToReloadJob(glm::ivec2 abs_pos, Mesh *mesh,
                              AABB bounding_box);

        /// Do the initial loading of the world
        void initialLoad(WorldParallelOperationsManager *thread_pool_manager);
        /// callback from a queue operation to tell what to do once chunk is
        /// generated Also does the chunk loading in the world
        void addMeshAndGPULoadChunkJob(std::shared_ptr<Chunk>);


        /// callback from a queue operation to tell what to do once chunk is
        /// generated Also does the chunk loading in the world
        void addMeshAndGPUReloadChunkJob(std::shared_ptr<Chunk>);

        /// callback from the world to reload chunks when edited
        void addChunkToReload(glm::ivec2);

      protected:
        /// The chunk position of the player just before this call
        glm::ivec2 lastPlayerChunkPos;

        /// The queue of meshes waiting to be loaded. Use the
        /// #meshes_to_load_lock !!!!
        std::queue<MeshToLoadBundle> meshes_to_load;
        /// The locker for #meshes_to_load
        std::mutex meshes_to_load_lock;


        /// The queue of meshes waiting to be reloaded. Use the
        /// #meshes_to_reload_lock !!!!
        std::queue<MeshToLoadBundle> meshes_to_reload;
        /// The locker for #meshes_to_reload
        std::mutex meshes_to_reload_lock;

        /// Whether chunks are stored in the world, and or loaded on the
        /// renderer
        ///
        /// Remember to use it in combination with #chunk_properties_lock !!!!
        std::unordered_map<glm::ivec2, ChunkControlProperties, KeyFuncs,
                           KeyFuncs>
            chunk_properties;
        /// Mutex for #chunk_properties
        std::mutex chunk_properties_lock;

        /// Width of the chunk map
        /// Typically 2* (render_distance + 1) + 1 as it include one chunk per
        /// player, render_distance on both side and one more line of chunk per
        /// side that is not rendered.
        int chunkWidth;

        /// The whole game
        Game *game;

        /// Move the world when the player change chunks
        void moveWorld(glm::ivec2 new_player_pos);
        /// Take one mehs out of the queue and load it on the gpu
        void loadOneMeshOnGpu();

        /// Take one mehs out of the queue and load it on the gpu
        void reloadOneMeshOnGpu();

        /// Remove one chunk from the gpu
        void removeChunkFromGPU(glm::ivec2 abs_pos);
        /// Remove one chunk from the world
        void removeChunkFromWorld(glm::ivec2 abs_pos);
};

#endif
