#ifndef CUBETEXTURE_H
#define CUBETEXTURE_H

#include <string>

enum class CubeAtlas { Air = 0, Grass = 1, Sand = 2, Stone = 3, Planks = 4 };


class CubeTexture
{

    public:
        CubeTexture(const std::string& path);
        virtual ~CubeTexture();
        // the actual factor by which to multiply to get the correct proportion  = (tile_width/texture_width)
        float FactorWidth() const;
        // the actual factor by which to multiply to get the correct proportion  = (tile_height/texture_height)
        float FactorHeight() const;

    protected:
        // the total texture size
        int realTextureWidth;
        int realTextureHeight;
        // the part of the texture that we use
        static constexpr float usedTextureWidth = 96;
        static constexpr float usedTextureHeight = 16;
        // the number of channels of the image
        int nbChannels;
        // the opengl texture number
        unsigned int id;
        bool correctlyLoad;

};

#endif
