#include "QuadRenderer.h"
#include "Shader.h"
#include <GL/gl.h>

QuadRenderer::QuadRenderer() {
    m_quadModel.loadData({
        {
            0.5f,  0.5f, 0.0f,  // top right
            0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  // bottom left
            -0.5f,  0.5f, 0.0f
        },
        {
        },
        {
            0, 1, 3,
            1, 2, 3,
        }
    });
}

void QuadRenderer::render(Shader &shader) {
    shader.use();
    m_quadModel.bindVAO();
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, &);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
