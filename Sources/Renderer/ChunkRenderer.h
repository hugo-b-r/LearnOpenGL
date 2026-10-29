#ifndef CHUNKRENDERER_H
#define CHUNKRENDERER_H

#include "Camera.h"
#include "World/World.h"
#include <CubeTexture.h>
#include <Model.h>
#include <Shader.h>
#include <World/ChunkMeshCreator.h>
#include <forward_list>
#include <mutex>
#include <queue>

class Chunk;

struct LocalizedBoundedModel {
        Model model;
        AABB boundingBox;
        glm::ivec2 pos;
        unsigned int nb_vertices;
        glm::vec3 center_pos;
};

struct ChunkToLoadOnGPUBundle {
        glm::ivec2 abs_pos;
};

class ChunkRenderer {
      public:
        ChunkRenderer();
        // void render(Chunk& c, Camera& cam, int nb_vertices, bool bindShader =
        // false, bool bindTexture = false);
        void render(Camera &cam);

        Shader &getShader();
        void bindTextureAndShader();

        CubeTexture texture;

        void addChunkToRender(glm::ivec2 abs_pos, World *w, AABB bounding_box,
                              Mesh *mesh);
        void removeChunkFromRender(glm::ivec2 abs_pos);
        void rebuildRenderChunk(glm::ivec2 abs_pos, World *w);

      private:
        Shader m_shader;

        std::forward_list<LocalizedBoundedModel> models_for_render;
        // std::mutex models_lock;

        std::queue<glm::ivec2> chunks_models_to_remove;
        std::mutex chunks_models_to_remove_lock;

        std::queue<ChunkToLoadOnGPUBundle> chunks_models_to_load_and_add;
        std::mutex chunks_models_to_load_and_add_lock;
};

#endif
