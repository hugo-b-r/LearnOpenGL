#include "ChunkMeshCreator.h"
#include "Chunk.h"
#include "CubeTexture.h"
#include "Mesh.h"
#include "utils.h"

#include <spdlog/spdlog.h>
#include <array>


struct NeighbooringFaces {
    void update(int x, int y, int z) {
         left = {x-1,y, z };
         right = {x+1, y, z};
         up = {x, y+1, z};
         down = {x, y-1, z};
         front = {x, y, z+1};
         rear = {x, y, z-1};
    }

    glm::ivec3 left;
    glm::ivec3 right;
    glm::ivec3 up;
    glm::ivec3 down;
    glm::ivec3 front;
    glm::ivec3 rear;
};


std::array<GLfloat, 48> ChunkMeshCreator::upFace(int select, float x, float y, float z) const {
   return std::array<GLfloat, 48>( {
    (x - (float)CHUNK_WIDTH/2.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f + 1.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f) * cube_length, 0.0f, (1.0f + select) * tex_vertical_factor,  0, 1, 0,
    (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f + 1.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f +1.f) * cube_length, 0.166667f, (0.0f + select) * tex_vertical_factor,  0, 1, 0,
    (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f + 1.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f) * cube_length, 0.166667f, (1.0f  + select) * tex_vertical_factor, 0, 1, 0,
    (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f + 1.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f +1.f) * cube_length, 0.166667f, (0.0f + select) * tex_vertical_factor, 0, 1, 0,
    (x - (float)CHUNK_WIDTH/2.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f + 1.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f) * cube_length, 0.0f, (1.0f + select) * tex_vertical_factor, 0, 1, 0,
    (x - (float)CHUNK_WIDTH/2.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f + 1.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f +1.f) * cube_length, 0.0f, (0.0f + select) * tex_vertical_factor , 0, 1, 0,

   });
}
std::array<GLfloat, 48> ChunkMeshCreator::downFace(int select, float x, float y, float z) const {
   return std::array<GLfloat, 48>({

       (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f +1.f) * cube_length, 1.0f, (0.0f + select) * tex_vertical_factor, 0, -1, 0,
       (x - (float)CHUNK_WIDTH/2.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f +1.f) * cube_length, 0.833333f, (0.0f + select) * tex_vertical_factor, 0, -1, 0,  // Ordre de coins ajusté
       (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f) * cube_length, 1.0f, (1.0f + select) * tex_vertical_factor , 0, -1, 0,
       (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f) * cube_length, 1.0f, (1.0f + select) * tex_vertical_factor , 0, -1, 0,
       (x - (float)CHUNK_WIDTH/2.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f +1.f) * cube_length, 0.833333f, (0.0f + select)  * tex_vertical_factor, 0, -1, 0,
              (x - (float)CHUNK_WIDTH/2.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f) * cube_length, 0.833333f, (1.0f + select) * tex_vertical_factor , 0, -1, 0,
   });
}
std::array<GLfloat, 48> ChunkMeshCreator::leftFace(int select, float x, float y, float z) const {
   return std::array<GLfloat, 48>( {
       (x - (float)CHUNK_WIDTH/2.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f) * cube_length, 0.5f, (1.0f + select) * tex_vertical_factor, -1, 0, 0,
       (x - (float)CHUNK_WIDTH/2.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f +1.f) * cube_length, 0.333333f, (1.0f + select) * tex_vertical_factor, -1, 0, 0,
       (x - (float)CHUNK_WIDTH/2.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f + 1.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f) * cube_length, 0.5f, (0.0f + select) * tex_vertical_factor, -1, 0, 0,
       (x - (float)CHUNK_WIDTH/2.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f + 1.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f +1.f) * cube_length, 0.333333f, (0.0f + select) * tex_vertical_factor, -1, 0, 0,
       (x - (float)CHUNK_WIDTH/2.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f + 1.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f) * cube_length, 0.5f, (0.0f + select) * tex_vertical_factor, -1, 0, 0,
       (x - (float)CHUNK_WIDTH/2.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f +1.f) * cube_length, 0.333333f, (1.0f + select) * tex_vertical_factor, -1, 0, 0,

   });
};
std::array<GLfloat, 48> ChunkMeshCreator::rightFace(int select, float x, float y, float z) const {
   return std::array<GLfloat, 48>( {
       (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f) * cube_length, 0.833333f, (1.0f + select) * tex_vertical_factor, 1, 0, 0, // Ordre de coins ajusté pour l'orientation
       (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f + 1.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f +1.f) * cube_length, 0.666667f, (0.0f + select) * tex_vertical_factor, 1, 0, 0,
       (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f +1.f) * cube_length, 0.666667f, (1.0f + select) * tex_vertical_factor, 1, 0, 0,
       (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f + 1.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f +1.f) * cube_length, 0.666667f, (0.0f + select) * tex_vertical_factor, 1, 0, 0,
       (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f) * cube_length, 0.833333f, (1.0f + select) * tex_vertical_factor, 1, 0, 0,
       (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f + 1.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f) * cube_length, 0.833333f, (0.0f + select) * tex_vertical_factor, 1, 0, 0,
   });
};
std::array<GLfloat, 48> ChunkMeshCreator::frontFace(int select, float x, float y, float z) const {
   return std::array<GLfloat, 48>( {
       (x - (float)CHUNK_WIDTH/2.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f +1.f) * cube_length, 0.166667f, (1.0f + select) * tex_vertical_factor, 0, 0, -1,
       (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f +1.f) * cube_length, 0.333333f, (1.0f + select) * tex_vertical_factor, 0, 0, -1,
       (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f + 1.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f +1.f) * cube_length, 0.333333f, (0.0f + select) * tex_vertical_factor, 0, 0, -1,
       (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f + 1.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f +1.f) * cube_length, 0.333333f, (0.0f + select) * tex_vertical_factor, 0, 0, -1,
       (x - (float)CHUNK_WIDTH/2.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f + 1.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f +1.f) * cube_length, 0.166667f, (0.0f + select) * tex_vertical_factor, 0, 0, -1,
       (x - (float)CHUNK_WIDTH/2.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f +1.f) * cube_length, 0.166667f, (1.0f + select) * tex_vertical_factor, 0, 0, -1,
   });
};
std::array<GLfloat, 48> ChunkMeshCreator::rearFace(int select, float x, float y, float z) const {
   return std::array<GLfloat, 48>( {
       (x - (float)CHUNK_WIDTH/2.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f) * cube_length, 0.666667f, (1.0f + select) * tex_vertical_factor, 0, 0, 1, // Ordre de coins inversé pour l'orientation
       (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f + 1.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f) * cube_length, 0.5f, (0.0f + select) * tex_vertical_factor, 0, 0, 1,
       (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f) * cube_length, 0.5f, (1.0f + select) * tex_vertical_factor, 0, 0, 1,
       (x - (float)CHUNK_WIDTH/2.f + 1.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f + 1.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f) * cube_length, 0.5f, (0.0f + select) * tex_vertical_factor, 0, 0, 1,
       (x - (float)CHUNK_WIDTH/2.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f) * cube_length, 0.666667f, (1.0f + select) * tex_vertical_factor, 0, 0, 1,
       (x - (float)CHUNK_WIDTH/2.f) * cube_width, (y - (float)CHUNK_HEIGHT/2.f + 1.f) * cube_height, (z - (float)CHUNK_LENGTH/2.f) * cube_length, 0.666667f, (0.0f + select) * tex_vertical_factor, 0, 0, 1,

   });
};


ChunkMeshCreator::ChunkMeshCreator(Chunk& c, CubeTexture& tex, World& w) : nb_vertices(0), tex_vertical_factor(tex.FactorHeight()), data(c), status(ChunkMeshBuildStatus::Running), m_world(w) {
    m_mesh = {{}, {}, {}};

}

int ChunkMeshCreator::buildMesh() {
    return buildMeshThreadFn();
}

Mesh ChunkMeshCreator::getMesh() {
    return m_mesh;
}

bool ChunkMeshCreator::isFinished() {
    return status == ChunkMeshBuildStatus::DoneNotLoaded ||status == ChunkMeshBuildStatus::DoneLoaded;
}

int ChunkMeshCreator::nbVertices() {
    return nb_vertices;
}



bool ChunkMeshCreator::shouldMakeFace(glm::ivec3 side_block) {
    int x = side_block.x, y = side_block.y, z = side_block.z;
    if (y < 0) {
        // under the sea
        return false;
    } else if (
        x < CHUNK_WIDTH && x >= 0 &&
        y < CHUNK_HEIGHT &&
        z < CHUNK_LENGTH && z >= 0
    ) {
        // inside the chunk
        auto block = data.getBlockAt(x, y, z);
        switch (block) {
            case CubeAtlas::Air:
            return true;
            case CubeAtlas::Leaves:
            return true;
            default:
            return false;
        }
        // if (block == CubeAtlas::Air ) {
        //     return true;
        // } else {
        //     return false;
        // }
    } else if ( y>= CHUNK_HEIGHT) {
        return true;
    } else {
        auto block = m_world.getBlockAt(fromInChunkToGlobalCoordinates(side_block, data.getChunkPos()));
        switch (block) {
            case CubeAtlas::Air:
            return true;
            case CubeAtlas::Leaves:
            return true;
            default:
            return false;
        }
    }
}

int ChunkMeshCreator::buildMeshThreadFn() {
    for (int i = 0; i < CHUNK_WIDTH; i++) {
      for (int j = 0; j < CHUNK_HEIGHT; j++) {
        for (int k = 0; k < CHUNK_LENGTH; k++) {
            NeighbooringFaces neighboors;
            neighboors.update(i, j, k);
            CubeAtlas block = data.getBlockAt(i, j, k);

            if (block == CubeAtlas::Air) {
                continue;
            }

            if (shouldMakeFace(neighboors.up)) {
                std::array<GLfloat, 48> temp = upFace((int)block, (float) i, (float) j, (float) k);

                m_mesh.vertexPositions.insert(m_mesh.vertexPositions.end(), temp.begin(), temp.end());
                nb_vertices+=6;
            }

            if (shouldMakeFace(neighboors.down)) {
                std::array<GLfloat, 48> temp = downFace((int)block, (float) i, (float) j, (float) k);

                m_mesh.vertexPositions.insert(m_mesh.vertexPositions.end(), temp.begin(), temp.end());
                nb_vertices+=6;
            }
            if (shouldMakeFace(neighboors.left)) {
                std::array<GLfloat, 48> temp = leftFace((int)block, (float) i, (float) j, (float) k);

                m_mesh.vertexPositions.insert(m_mesh.vertexPositions.end(), temp.begin(), temp.end());
                nb_vertices+=6;
            }
            if (shouldMakeFace(neighboors.right)) {
                std::array<GLfloat, 48> temp = rightFace((int)block, (float) i, (float) j, (float) k);

                m_mesh.vertexPositions.insert(m_mesh.vertexPositions.end(), temp.begin(), temp.end());
                nb_vertices+=6;
            }
            if (shouldMakeFace(neighboors.front)) {
                std::array<GLfloat, 48> temp = frontFace((int)block, (float) i, (float) j, (float) k);

                m_mesh.vertexPositions.insert(m_mesh.vertexPositions.end(), temp.begin(), temp.end());
                nb_vertices+=6;
            }
            if (shouldMakeFace(neighboors.rear)) {
                std::array<GLfloat, 48> temp = rearFace((int)block, (float) i, (float) j, (float) k);

                m_mesh.vertexPositions.insert(m_mesh.vertexPositions.end(), temp.begin(), temp.end());
                nb_vertices+=6;
            }
        }
      }
    }
    status = ChunkMeshBuildStatus::DoneLoaded;
    return nb_vertices;
}
