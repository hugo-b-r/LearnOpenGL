#ifndef CHUNK_H
#define CHUNK_H

#include "Camera.h"
#include "CubeTexture.h"
#include "Mathematics/AABB.h"
#include "Mesh.h"

#include <glm/glm.hpp>
#include <memory>
#include <vector>

const int CHUNK_WIDTH = 16;
const int CHUNK_HEIGHT = 256;
const int CHUNK_LENGTH = 16;

enum class ChunkDataLoadStatus {
        None,
        Running,
        FinishedNotLoaded,
        FinishedLoaded
};

class Chunk;
class ChunkMeshCreator;
class ChunkRenderer;
class World;
class RemoveSafeChunk;
enum class ChunkMeshBuildStatus;

struct NeighboringChunks {
        std::shared_ptr<Chunk> x_p; // in x positive direction
        std::shared_ptr<Chunk> x_m; // in x negative direction
        std::shared_ptr<Chunk> y_p; // in y positive direction
        std::shared_ptr<Chunk> y_m; // in y negative direction
};

class Chunk {
      public:
        Chunk();
        // creates a chunk at pos pos in chunk coordinates among other chunks
        Chunk(glm::ivec2 pos);
        // creates a chunk at pos (x, y) in chunk coordinates among other chunks
        Chunk(int x, int y);

        virtual ~Chunk();
        CubeAtlas getBlockAt(int x, int y, int z);
        CubeAtlas getBlockAt(glm::ivec3 pos);
        void setBlockAt(int x, int y, int z, CubeAtlas ca);

        glm::ivec2 getChunkPos();
        glm::ivec3 getChunkPosCenter();
        glm::vec3 getChunkPosCenter3();

        void load(World *w);
        bool isLoaded();

        void createMesh(World *w, CubeTexture &ct);
        bool hasMesh();

        // void loadMeshOnGPU();
        // bool hasOnGPUMesh();

        // void bindModel();

        void draw(Camera &cam, ChunkRenderer &renderer);

        void createBoundingBox();

        AABB &getBoundingBox();
        Mesh m_mesh;

      protected:
        std::vector<CubeAtlas> blocks;
        ChunkDataLoadStatus m_loadStatus;
        ChunkMeshBuildStatus m_meshBuildStatus;
        // Model m_model;
        glm::ivec2 chunkPos;
        glm::mat4 m_modelMat;
        glm::mat4 m_viewMat;
        int m_numberVertices;
        AABB m_boundingBox;
};

#endif
