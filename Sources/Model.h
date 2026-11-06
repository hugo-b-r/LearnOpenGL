#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"
#include <vector>


/// A mesh on the GPU (OpenGL)
class Model {
    public:

        /// Some constructor
        Model() = default;
        /// Default constructor
        Model(Mesh* mesh);
        /// Destructor
        ~Model();


        /// Loads the mesh on the gpu using \a dimensions
        void loadData(std::vector<int> dimensions = {});

        /// Adds Vertex-Array Object on the gpu
        void genVAO();
        /// Adds Element Buffer Object on gpu
        void addEBO(const std::vector<GLuint>& indices);
        /// Adds Vertex Buffer object on gpu
        void addVBO(int dimensions, const std::vector<GLfloat>& data);
        /// Binds the Vertex Array Object (for rendering purposes for example)
        void bindVAO();

    private:
        unsigned int m_vboCount;
        unsigned int m_VBO;
        unsigned int m_VAO;
        unsigned int m_EBO;
        unsigned int m_indicesCount;

        bool vbo_generated = false;
        bool vao_generation = false;
        bool ebo_generated = false;

        Mesh * mesh;

        std::vector<GLuint> m_buffers;


};

#endif
