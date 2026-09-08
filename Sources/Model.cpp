#include "Model.h"

#include <vector>

#include "Mesh.h"
#include "utils.h"

Model::Model(const Mesh& mesh) : m_vboCount(0) {
    loadData(mesh, {});
}

Model::~Model() {


}

void Model::loadData(const Mesh& mesh, std::vector<int> dimensions) {
    m_vboCount = 0;
    genVAO();
    for (int& i : dimensions) {
        addVBO(i, mesh.vertexPositions);
    }
    if (mesh.textureCoords.size()>0)
        addVBO(2, mesh.textureCoords);
    if (mesh.indices.size()>0)
        addEBO(mesh.indices);
}

void Model::bindVAO() {
     glBindVertexArray(m_VAO);
}

void Model::genVAO()
{
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
}


void Model::addVBO(int dimensions, const std::vector<GLfloat>& data) {
    UNUSED(dimensions);
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(m_vboCount, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(m_vboCount++);
    m_buffers.push_back(m_vboCount);
    glVertexAttribPointer(m_vboCount, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(m_vboCount++);
    m_buffers.push_back(m_vboCount);
    glVertexAttribPointer(m_vboCount, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(m_vboCount++);
    m_buffers.push_back(m_vboCount);
}


void Model::addEBO(const std::vector<GLuint>& indices)
{
    m_indicesCount = (unsigned int) (indices.size());
    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(GLuint),
                 indices.data(),
                 GL_STATIC_DRAW);
}
