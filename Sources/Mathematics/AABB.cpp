#include "AABB.h"


AABB::AABB(glm::vec3 vmi, glm::vec3 vma) : vmin (vmi), vmax(vma) {
}


void AABB::update(glm::vec3 vmi, glm::vec3 vma) {
    vmin = vmi;
    vmax = vma;
}

glm::vec3 AABB::getVmin() const {
    return vmin;
}

glm::vec3 AABB::getVmax() const {
    return vmax;
}
