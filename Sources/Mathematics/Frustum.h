#ifndef FRUSTUM_H
#define FRUSTUM_H

#include <glm/glm.hpp>
#include <vector>

#include "AABB.h"

/// A simple camera frustum with the capacity to test if AABB is inside.
class Frustum {
      public:
        /// Constructor
        Frustum(glm::mat4 projViewMat);
        /// Constructor
        Frustum();
        /// Constructor of copy
        Frustum(const Frustum &f);
        /// = operator
        Frustum &operator=(const Frustum &f);

        /// update frstum for a niew matrices 
        void updateFrom(glm::mat4 invProjViewMat);
        /// Whether an #AABB is visible in a frustum
        bool IsAabbVisible(AABB const &box);

      protected:
        /// planes extracted from anything
        std::vector<glm::vec4> m_planes;
};

#endif
