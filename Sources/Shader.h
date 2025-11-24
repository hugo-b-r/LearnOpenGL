#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

class Shader {
public:
  Shader(const char *vertexPath, const char *fragmentPath);
  void use();

protected:
  unsigned int id;
};

#endif