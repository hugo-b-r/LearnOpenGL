#include "CubeTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../deps/stb_image.h"

#include "../deps/glad/include/glad/glad.h"
#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>




// textures de cube sont de 16 par 16 pour chaque face donc 16x(16x6)
CubeTexture::CubeTexture(const std::string& path)
{
    correctlyLoad = false;
    unsigned char *data = stbi_load(path.c_str(), &realTextureWidth, &realTextureHeight, &nbChannels, 0);

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);


    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, usedTextureWidth, usedTextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        correctlyLoad = true;
    }
    else
    {
        spdlog::error("Failed to load texture");
    }
    stbi_image_free(data);
}

CubeTexture::~CubeTexture()
{
    glDeleteTextures(1, &id);
}


float CubeTexture::FactorWidth() const
{
    return usedTextureWidth / realTextureWidth;

}

float CubeTexture::FactorHeight() const
{
    return usedTextureHeight / realTextureHeight;
}
