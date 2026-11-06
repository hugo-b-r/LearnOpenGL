#ifndef SHADER_H
#define SHADER_H

#include "../deps/glad/include/glad/glad.h" // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>
#include <string>


/// An OpenGL Shader
class Shader {
      public:
        /// The opengl id of the shader
        unsigned int id;
        /// Default constructor
        Shader(const char *vertexPath, const char *fragmentPath);
        /// Default destructor
        ~Shader();
        /// bind the shader to OpenGL
        void use();

        /// Utility uniform functions : set boolean uniform
        void setBool(const std::string &name, bool value) const;
        /// Utility uniform functions : set integer uniform
        void setInt(const std::string &name, int value) const;
        /// Utility uniform functions : set floating uniform
        void setFloat(const std::string &name, float value) const;

        /// Utility uniform functions : set glm vec2 uniform
        void setVec2(const std::string &name, const glm::vec2 &value) const;
        /// Utility uniform functions : set vec2 uniform
        void setVec2(const std::string &name, float x, float y) const;
        /// Utility uniform functions : set glm vec3 uniform
        void setVec3(const std::string &name, const glm::vec3 &value) const;
        /// Utility uniform functions : set vec3 uniform
        void setVec3(const std::string &name, float x, float y, float z) const;
        /// Utility uniform functions : set glm vec4 uniform
        void setVec4(const std::string &name, const glm::vec4 &value) const;
        /// Utility uniform functions : set vec4 uniform
        void setVec4(const std::string &name, float x, float y, float z,
                     float w) const;

        /// Utility uniform functions : set glm mat2 uniform
        void setMat2(const std::string &name, const glm::mat2 &mat) const;
        /// Utility uniform functions : set glm mat3 uniform
        void setMat3(const std::string &name, const glm::mat3 &mat) const;
        /// Utility uniform functions : set glm mat4 uniform
        void setMat4(const std::string &name, const glm::mat4 &mat) const;

      protected:
};

#endif
