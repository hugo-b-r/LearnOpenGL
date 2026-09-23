#ifndef CHUNKRENDERER_H
#define CHUNKRENDERER_H

#include "Camera.h"
#include "World/Chunk.h"
#include <World/ChunkMeshCreator.h>
#include <CubeTexture.h>
#include <Model.h>
#include <Shader.h>

class Chunk;


class ChunkRenderer {
public:
    ChunkRenderer();
    // void render(Chunk& c, Camera& cam, int nb_vertices, bool bindShader = false, bool bindTexture = false);
    void render(Model& m, Camera& cam, int nb_vertices, glm::vec3 chunk_center, bool bindShader = false, bool bindTexture = false);

    Shader& getShader();
    void bindTextureAndShader();

    
    CubeTexture texture;

private:
    Shader m_shader;
};

#endif
