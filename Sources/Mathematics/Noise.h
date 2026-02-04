#ifndef NOISE_H
#define NOISE_H

#include <glm/glm.hpp>

class Noise {
#include "Noise.h"

public:
  static float perlin(float x, float y, float z);
  static float turbulence(glm::vec2 p, float amplitude, float fbase, float attenuation, int noctave);
  static float ridged( glm::vec2 p);

protected:
  static glm::vec2 randomGradient(int ix, int iy);

  // Computes the dot product of the distance and gradient vectors.
  static float dotGridGradient(int ix, int iy, float x, float y);

  static float interpolate(float a0, float a1, float w);




  // Sample Perlin noise at coordinates x, y
};

#endif
