#ifndef CUBETEXTURE_H
#define CUBETEXTURE_H

#include <glad/glad.h>
#include <string>

/// What block it is
enum class CubeAtlas {
        Air = 0,
        Grass = 1,
        Dirt = 2,
        Sand = 3,
        Stone = 4,
        Planks = 5,
        Wood = 6,
        Leaves = 7
};

/// Holds data for the textures of a cube (actually of the world). It is a big
/// array of textures, that looks a bit like that:
///
/// ```
/// 123456
/// 123456
/// 123456
/// 123456
/// 123456
///  ....
/// 123456
/// ```
/// With 1, 2, 3, 4, 5 & 6 being one face of a cube.
class CubeTexture {

      public:
        /// The default cnstructor
        CubeTexture(const std::string &path);
        virtual ~CubeTexture();
        /// the actual factor by which to multiply to get the correct proportion
        /// = (tile_width/texture_width)
        float FactorWidth() const;
        /// the actual factor by which to multiply to get the correct proportion
        /// = (tile_height/texture_height)
        float FactorHeight() const;

        /// To bind the textures when rendering
        void bind();

      protected:
        /// The total texture width
        int realTextureWidth;
        /// The total texture height
        int realTextureHeight;
        /// The part of the texture that we use
        static constexpr float usedTextureWidth = 96;
        /// The part of the texture that we use
        static constexpr float usedTextureHeight = 16;
        /// The number of channels of the image
        int nbChannels;
        /// The opengl texture number
        GLuint id;
        /// To kno wif the image was correctly loaded or not
        bool correctlyLoad;
};

#endif
