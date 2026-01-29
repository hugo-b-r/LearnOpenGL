#ifndef PLANE_H
#define PLANE_H


#include <glm/glm.hpp>

struct Plane
{
  glm::vec3 normal = { 0.f, 1.f, 0.f};
  glm::vec3 point = { 0.f, 0.f, 0.f};

};

#endif
