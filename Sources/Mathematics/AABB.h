/* An Axis-Aligned Bounding Box for collision detection */


#ifndef AABB_H
#define AABB_H

#include <glm/glm.hpp>

class AABB {
    public:
        glm::vec3 vmin;
        glm::vec3 vmax;

};

#endif
