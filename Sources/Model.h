#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"
#include <vector>

class Model {
    public:
        Model() = default;
        Model(const Mesh& mesh);
        ~Model();

        // Model(const Model& other);
        // Model& operator=(const Model& other) const;

        void loadData(const Mesh& mesh, std::vector<int> dimensions = {});

        void genVAO();
        void addEBO(const std::vector<GLuint>& indices);
        void addVBO(int dimensions, const std::vector<GLfloat>& data);
        void bindVAO();

    private:
        unsigned int m_vboCount;
        unsigned int m_VBO;
        unsigned int m_VAO;
        unsigned int m_EBO;
        unsigned int m_indicesCount;

        std::vector<GLuint> m_buffers;


};

#endif
