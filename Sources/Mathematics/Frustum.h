#ifndef FRUSTUM_H
#define FRUSTUM_H

#include <vector>
#include <glm/glm.hpp>

#include "AABB.h"

/* A simple camera frustum with the capacity to test if AABB is inside.*/
class Frustum
{
    public:
    Frustum(glm::mat4 projViewMat);
    Frustum();
    Frustum(const Frustum & f);
    Frustum& operator=(const Frustum& f);

    void updateFrom(glm::mat4 invProjViewMat);
    bool IsAabbVisible(AABB const & box );
    protected:
    /* planes extracted from anything */
    std::vector<glm::vec4> m_planes;

};

#endif
