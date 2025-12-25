#ifndef CHUNKRENDERER_H
#define CHUNKRENDERER_H

#include "Camera.h"
#include "Chunk.h"
#include "ChunkMeshCreator.h"
#include "CubeTexture.h"
#include "Model.h"
#include "Shader.h"

class ChunkRenderer {
public:
    ChunkRenderer(Chunk& c, glm::vec3 pos);
    void render(Shader &shader, Camera &cam);

private:
  Chunk& m_chunk_data;
  Model m_model;
  bool m_modelLoaded;
  glm::mat4 m_view_mat;
  int m_nb_vertices;
  CubeTexture m_texture;
  ChunkMeshCreator m_chunkMeshCreator;
};

#endif
