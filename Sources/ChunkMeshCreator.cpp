#include "ChunkMeshCreator.h"
#include "Chunk.h"
#include "CubeTexture.h"
#include "Mesh.h"
#include <spdlog/spdlog.h>

struct NeighbooringFaces {
    void update(int x, int y, int z) {
         left = {x-1,y, z };
         right = {x+1, y, z};
         up = {x, y+1, z};
         down = {x, y-1, z};
         front = {x, y, z-1};
         rear = {x, y, z+1};
    }

    glm::vec3 left;
    glm::vec3 right;
    glm::vec3 up;
    glm::vec3 down;
    glm::vec3 front;
    glm::vec3 rear;
};


std::vector<GLfloat> ChunkMeshCreator::upFace(int select, float x, float y, float z) const {
   return std::vector<GLfloat>( {
    (x - (float)CHUNK_WIDTH/2.f) * cube_width, 0.5f * cube_height, -0.5f * cube_length, 0.0f, (1.0f + select) * tex_vertical_factor ,
    (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, 0.5f * cube_height, -0.5f * cube_length, 0.166667f, (1.0f  + select) * tex_vertical_factor ,
    (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, 0.5f * cube_height, 0.5f * cube_length, 0.166667f, (0.0f + select) * tex_vertical_factor ,
    (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, 0.5f * cube_height, 0.5f * cube_length, 0.166667f, (0.0f + select) * tex_vertical_factor ,
    (x - (float)CHUNK_WIDTH/2.f) * cube_width, 0.5f * cube_height, 0.5f * cube_length, 0.0f, (0.0f + select) * tex_vertical_factor ,
    (x - (float)CHUNK_WIDTH/2.f) * cube_width, 0.5f * cube_height, -0.5f * cube_length, 0.0f, (1.0f + select) * tex_vertical_factor
   });
}
std::vector<GLfloat> ChunkMeshCreator::downFace(int select, float x, float y, float z) const {
   return std::vector<GLfloat>({
       (x - (float)CHUNK_WIDTH/2.f) * cube_width, -0.5f * cube_height, 0.5f * cube_length, 0.833333f, (0.0f + select) * tex_vertical_factor , // Ordre de coins ajusté
       (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, -0.5f * cube_height, 0.5f * cube_length, 1.0f, (0.0f + select) * tex_vertical_factor ,
       (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, -0.5f * cube_height, -0.5f * cube_length, 1.0f, (1.0f + select) * tex_vertical_factor ,
       (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, -0.5f * cube_height, -0.5f * cube_length, 1.0f, (1.0f + select) * tex_vertical_factor ,
       (x - (float)CHUNK_WIDTH/2.f) * cube_width, -0.5f * cube_height, -0.5f * cube_length, 0.833333f, (1.0f + select) * tex_vertical_factor ,
       (x - (float)CHUNK_WIDTH/2.f) * cube_width, -0.5f * cube_height, 0.5f * cube_length, 0.833333f, (0.0f + select)  * tex_vertical_factor
   });
}
std::vector<GLfloat> ChunkMeshCreator::leftFace(int select, float x, float y, float z) const {
   return std::vector<GLfloat>( {
       (x - (float)CHUNK_WIDTH/2.f) * cube_width, -0.5f * cube_height, 0.5f * cube_length, 0.333333f, (1.0f + select) * tex_vertical_factor ,
       (x - (float)CHUNK_WIDTH/2.f) * cube_width, -0.5f * cube_height, -0.5f * cube_length, 0.5f, (1.0f + select) * tex_vertical_factor ,
       (x - (float)CHUNK_WIDTH/2.f) * cube_width, 0.5f * cube_height, -0.5f * cube_length, 0.5f, (0.0f + select) * tex_vertical_factor ,
       (x - (float)CHUNK_WIDTH/2.f) * cube_width, 0.5f * cube_height, -0.5f * cube_length, 0.5f, (0.0f + select) * tex_vertical_factor ,
       (x - (float)CHUNK_WIDTH/2.f) * cube_width, 0.5f * cube_height, 0.5f * cube_length, 0.333333f, (0.0f + select) * tex_vertical_factor ,
       (x - (float)CHUNK_WIDTH/2.f) * cube_width, -0.5f * cube_height, 0.5f * cube_length, 0.333333f, (1.0f + select) * tex_vertical_factor

   });
};
std::vector<GLfloat> ChunkMeshCreator::rightFace(int select, float x, float y, float z) const {
   return std::vector<GLfloat>( {
       (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, -0.5f * cube_height, -0.5f * cube_length, 0.833333f, (1.0f + select) * tex_vertical_factor , // Ordre de coins ajusté pour l'orientation
       (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, -0.5f * cube_height, 0.5f * cube_length, 0.666667f, (1.0f + select) * tex_vertical_factor ,
       (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, 0.5f * cube_height, 0.5f * cube_length, 0.666667f, (0.0f + select) * tex_vertical_factor ,
       (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, 0.5f * cube_height, 0.5f * cube_length, 0.666667f, (0.0f + select) * tex_vertical_factor ,
       (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, 0.5f * cube_height, -0.5f * cube_length, 0.833333f, (0.0f + select) * tex_vertical_factor ,
       (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, -0.5f * cube_height, -0.5f * cube_length, 0.833333f, (1.0f + select) * tex_vertical_factor ,
   });
};
std::vector<GLfloat> ChunkMeshCreator::frontFace(int select, float x, float y, float z) const {
   return std::vector<GLfloat>( {
       (x - (float)CHUNK_WIDTH/2.f) * cube_width, -0.5f * cube_height, 0.5f * cube_length, 0.166667f, (1.0f + select) * tex_vertical_factor ,
       (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, -0.5f * cube_height, 0.5f * cube_length, 0.333333f, (1.0f + select) * tex_vertical_factor ,
       (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, 0.5f * cube_height, 0.5f * cube_length, 0.333333f, (0.0f + select) * tex_vertical_factor ,
       (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, 0.5f * cube_height, 0.5f * cube_length, 0.333333f, (0.0f + select) * tex_vertical_factor ,
       (x - (float)CHUNK_WIDTH/2.f) * cube_width, 0.5f * cube_height, 0.5f * cube_length, 0.166667f, (0.0f + select) * tex_vertical_factor ,
       (x - (float)CHUNK_WIDTH/2.f) * cube_width, -0.5f * cube_height, 0.5f * cube_length, 0.166667f, (1.0f + select) * tex_vertical_factor
   });
};
std::vector<GLfloat> ChunkMeshCreator::rearFace(int select, float x, float y, float z) const {
   return std::vector<GLfloat>( {
       (x - (float)CHUNK_WIDTH/2.f) * cube_width, -0.5f * cube_height, -0.5f * cube_length, 0.666667f, (1.0f + select) * tex_vertical_factor , // Ordre de coins inversé pour l'orientation
       (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, -0.5f * cube_height, -0.5f * cube_length, 0.5f, (1.0f + select) * tex_vertical_factor ,
       (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, 0.5f * cube_height, -0.5f * cube_length, 0.5f, (0.0f + select) * tex_vertical_factor ,
       (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, 0.5f * cube_height, -0.5f * cube_length, 0.5f, (0.0f + select) * tex_vertical_factor ,
       (x - (float)CHUNK_WIDTH/2.f) * cube_width, 0.5f * cube_height, -0.5f * cube_length, 0.666667f, (0.0f + select) * tex_vertical_factor ,
       (x - (float)CHUNK_WIDTH/2.f) * cube_width, -0.5f * cube_height, -0.5f * cube_length, 0.666667f, (1.0f + select) * tex_vertical_factor ,

   });
};


ChunkMeshCreator::ChunkMeshCreator() : m_mesh({{}, {}, {}}) {}

ChunkMeshCreator::ChunkMeshCreator(Chunk& c, CubeTexture& tex) {
    m_mesh = {{}, {}, {}};
    buildMesh(m_mesh, c);
}

int ChunkMeshCreator::buildMesh(Mesh &m, Chunk &c) {
    int nb_vertices = 0;
  for (int i = 0; i < CHUNK_WIDTH; i++) {
    for (int j = 0; j < CHUNK_LENGTH; j++) {
      for (int k = 0; k < CHUNK_HEIGHT; k++) {
          NeighbooringFaces neighboors;
          neighboors.update(i, j, k);
          CubeAtlas block = c.getBlockAt(i, j, k);

          if (block == CubeAtlas::Air) {
              continue;
          }

          if (shouldMakeFace(c, neighboors.up)) {
              std::vector<GLfloat> temp = upFace((int)block);
              m.vertexPositions.insert(m.vertexPositions.end(), temp.begin(), temp.end());
              nb_vertices+=6;
          }

          if (shouldMakeFace(c, neighboors.down)) {
              std::vector<GLfloat> temp = downFace((int)block);
              m.vertexPositions.insert(m.vertexPositions.end(), temp.begin(), temp.end());
              nb_vertices+=6;
          }
          if (shouldMakeFace(c, neighboors.left)) {
              std::vector<GLfloat> temp = leftFace((int)block);
              m.vertexPositions.insert(m.vertexPositions.end(), temp.begin(), temp.end());
              nb_vertices+=6;
          }
          if (shouldMakeFace(c, neighboors.right)) {
              std::vector<GLfloat> temp = rightFace((int)block);
              m.vertexPositions.insert(m.vertexPositions.end(), temp.begin(), temp.end());
              nb_vertices+=6;
          }
          if (shouldMakeFace(c, neighboors.front)) {
              std::vector<GLfloat> temp = frontFace((int)block);
              m.vertexPositions.insert(m.vertexPositions.end(), temp.begin(), temp.end());
              nb_vertices+=6;
          }
          if (shouldMakeFace(c, neighboors.rear)) {
              std::vector<GLfloat> temp = rearFace((int)block);
              m.vertexPositions.insert(m.vertexPositions.end(), temp.begin(), temp.end());
              nb_vertices+=6;
          }

      }
    }
  }
  return nb_vertices;
}

bool ChunkMeshCreator::shouldMakeFace(Chunk &c, glm::vec3 side_block) {
  if (side_block.x >= CHUNK_WIDTH || side_block.x < 0 ||
      side_block.y >= CHUNK_HEIGHT || side_block.y < 0 ||
      side_block.z >= CHUNK_LENGTH || side_block.z < 0 ||
      c.getBlockAt(side_block.x, side_block.y, side_block.z) ==
          CubeAtlas::Air) {
    return true;
  } else {
    return false;
  }
}
