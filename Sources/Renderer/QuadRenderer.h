#ifndef QUADRENDERER_H
#define QUADRENDERER_H

#include "Camera.h"
#include "Model.h"
#include "Shader.h"

/// A renderer for a quad
class QuadRenderer {
      public:
        /// default constructor
        QuadRenderer();
        /// render the quad to opengl context
        void render(Shader &shader, Camera &cam);

      private:
        /// The model for the quald
        Model m_quadModel;
        /// model transofrmation matrix
        glm::mat4 m_transforModel;
};

#endif // QUADRENDERER_H
