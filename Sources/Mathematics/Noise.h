#ifndef NOISE_H
#define NOISE_H

#include <glm/glm.hpp>


/// Main class to do some noise
class Noise {
#include "Noise.h"

public:
  /// Perlin noise as from Ken Perlin implementation.
  ///
  ///
  /// Translated from the [java version](https://mrl.cs.nyu.edu/~perlin/noise/).
  static float perlin(float x, float y, float z);

  /// An accumulation of perlin noises.
  static float turbulence(glm::vec2 p, float amplitude, float fbase, float attenuation, int noctave);
  /// Cuting the noise with ridges.
  static float ridged( glm::vec2 p);

protected:
  /// Random. Whatever.
  static glm::vec2 randomGradient(int ix, int iy);

  /// Computes the dot product of the distance and gradient vectors.
  static float dotGridGradient(int ix, int iy, float x, float y);
  /// Interpolate between Two floats.
  static float interpolate(float a0, float a1, float w);
};

#endif
