/* An Axis-Aligned Bounding Box for collision detection */


#ifndef AABB_H
#define AABB_H

#include <glm/glm.hpp>

class AABB {
    public:
        AABB(glm::vec3 vmi = glm::vec3(0, 0, 0), glm::vec3 vma = glm::vec3(0, 0, 0));
        void update(glm::vec3 vmi, glm::vec3 vma);
        glm::vec3 getVmin() const;
        glm::vec3 getVmax() const;
    protected:
        glm::vec3 vmin;
        glm::vec3 vmax;

};

#endif
