#ifndef UTILS_H
#define UTILS_H

#include <string>
#include "../deps/glm/glm/glm.hpp"

#define UNUSED(x) (void)x

std::string mat4_to_string(const glm::mat4& matrix);

#endif
