#ifndef CHUNK_H
#define CHUNK_H

#include "ChunkMeshCreator.h"
#include "CubeTexture.h"
#include "Mesh.h"
#include "Model.h"
#include "Renderer/ChunkRenderer.h"

#include <glm/glm.hpp>
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

class ChunkMeshCreator;
class ChunkRenderer;
enum class ChunkMeshBuildStatus;

class Chunk {
public:
  Chunk();
  Chunk(glm::ivec2 pos);
  CubeAtlas getBlockAt(int x, int y, int z);
  void setBlockAt(int x, int y, int z, CubeAtlas ca);

  glm::ivec2 getChunkPos();
  glm::ivec2 getChunkPosCenter();
  glm::vec3 getChunkPosCenter3();

  void load();
  bool isLoaded();

  void createMesh();
  bool hasMesh();

  void loadMeshOnGPU();
  bool hasOnGPUMesh();

  void bindModel();

  void draw(Camera &cam, ChunkRenderer &renderer);

protected:
  std::vector<CubeAtlas> blocks;
  ChunkDataLoadStatus m_loadStatus;
  ChunkMeshBuildStatus m_meshBuildStatus;
  Mesh m_mesh;
  Model m_model;
  glm::ivec2 chunkPos;
  glm::mat4 m_modelMat;
  glm::mat4 m_viewMat;
  int m_numberVertices;
};

#endif
