#ifndef PLANE_H
#define PLANE_H


#include <glm/glm.hpp>


/// Represents a basic square plane in space
struct Plane
{
  /// Plane orientation
  glm::vec3 normal = { 0.f, 1.f, 0.f};
  /// center of the plane
  glm::vec3 point = { 0.f, 0.f, 0.f};

};

#endif
