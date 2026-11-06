#ifndef CHUNKRENDERER_H
#define CHUNKRENDERER_H

#include "Camera.h"
#include "World/World.h"
#include "utils.h"
#include <CubeTexture.h>
#include <Model.h>
#include <Shader.h>
#include <World/ChunkMeshCreator.h>
#include <forward_list>
#include <mutex>
#include <queue>
#include <unordered_set>

const glm::mat4 view_mat(1.0f);

class Chunk;

/// Model with position in space for rendering
struct LocalizedBoundedModel {
        /// The actual model
        Model model;
        /// The model's bounding box
        AABB boundingBox;
        /// The chunk space position
        glm::ivec2 pos;
        /// Number of vertices to render
        unsigned int nb_vertices;
        /// Position of the center of the chunk
        glm::vec3 center_pos;
};

/// Pos of chunk to load on the GPU
struct ChunkToLoadOnGPUBundle {
        /// pos of chunk in chunk's grid
        glm::ivec2 abs_pos;
};

/// An OpenGL based renderer
class ChunkRenderer {
      public:
        /// Default constructor
        ChunkRenderer();
        /// Render the chunks on the screen
        void render(Camera &cam);

        /// Return shader
        Shader &getShader();

        /// the texture used for chunks
        CubeTexture texture;

        /// Add one chunk to the render loading queue
        void addChunkToRender(glm::ivec2 abs_pos, AABB bounding_box,
                              Mesh *mesh);

        /// Remove one chunk removal if one in queue
        ///
        /// It is used to remove chunks synchronously
        void doOneQueuedChunkRemovalIfNeeded();
        /// Add one chunk to remove queue
        void sendCommandToRemoveChunk(glm::ivec2 chunk_pos);

        /// Tells whether a chunk is in render or not
        bool isChunkInRenderer(glm::ivec2 chunk_pos);

      private:
        /// Bind texture and shader for rendering
        void bindTextureAndShader();

        /// The shader of the chunks
        Shader m_shader;

        /// A linked list of models to render
        std::forward_list<LocalizedBoundedModel> models_for_render;

        /// Chunks queue to remove
        std::queue<glm::ivec2> chunks_models_to_remove;
        /// Locker for #chunks_model_to_remove
        std::mutex chunks_models_to_remove_lock;

        /// Queue of chunks to add
        std::queue<ChunkToLoadOnGPUBundle> chunks_models_to_load_and_add;
        /// locker for #chunks_models_to_load_and_add
        std::mutex chunks_models_to_load_and_add_lock;

        /// keeps tracks of loaded chunks
        std::unordered_set<glm::ivec2, KeyFuncs, KeyFuncs> loaded_pos;
        /// Locker for #loaded_pos
        std::mutex loaded_pos_lock;
        /// Remove one chunk from the renderer chained list
        void removeChunkFromRender(glm::ivec2 abs_pos);
};

#endif
