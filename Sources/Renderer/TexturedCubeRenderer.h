#ifndef TEXTUREDCUBERENDERER_H
#define TEXTUREDCUBERENDERER_H

#include "Camera.h"
#include "CubeTexture.h"
#include "Model.h"
#include "Shader.h"
#include <tuple>

/// Distinguish faces of a cube
enum class CubeOrientation { Up, Down, Left, Right, Front, Rear };

/// rendering many cubes
class TexturedCubeRenderer {
      public:
        /// Default constructor
        TexturedCubeRenderer(CubeAtlas ca = CubeAtlas::Grass);
        /// Render the cube to the screen, takes care of binding neede opengl
        /// stuff
        void render(Shader &shader, Camera &cam);
        /// Add a cube to render
        void add(glm::vec3 pos = glm::vec3(0.f),
                 CubeOrientation orientation = CubeOrientation::Front);

      private:
        Model m_quadModel;

        bool m_rotating;
        float width = 1;
        float height = 1;
        float length = 1;
        CubeTexture tex;
        // cube atlas, view, model
        std::vector<std::tuple<glm::mat4, glm::mat4>> cubes;
};

#endif
