#include "Model.h"

Model::Model(const Mesh& mesh) {
    loadData(mesh);
}

Model::~Model() {

}

void Model::loadData(const Mesh& mesh) {
    genVAO();
    addVBO(3, mesh.vertexPositions);
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
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, dimensions, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
}


void Model::addEBO(const std::vector<GLuint>& indices)
{
    m_indicesCount = indices.size();
    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(GLuint),
                 indices.data(),
                 GL_STATIC_DRAW);
}
