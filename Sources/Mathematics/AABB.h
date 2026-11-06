/* An Axis-Aligned Bounding Box for collision detection */


#ifndef AABB_H
#define AABB_H

#include <glm/glm.hpp>

/// An Axis Aligned Bounding Box
///
/// Look at the [wikipedia link](https://en.wikipedia.org/wiki/Minimum_bounding_box#Axis-aligned_minimum_bounding_box) for more informations.
/// 
class AABB {
    public:
        /// Basic constructor
        /// \param vmi minimal vertex of the AABB
        /// \param vma maximal vertex of the AABB
        AABB(glm::vec3 vmi = glm::vec3(0, 0, 0), glm::vec3 vma = glm::vec3(0, 0, 0));

        /// Change value of the AABB
        /// \param vmi minimal vertex of the AABB
        /// \param vma maximal vertex of the AABB
        void update(glm::vec3 vmi, glm::vec3 vma);

        /// Getter for vmi / minimal vertex of AABB
        glm::vec3 getVmin() const;
        /// Getter for vma / maximal vertex of AABB
        glm::vec3 getVmax() const;
    protected:
        /// minimal vertex of AABB
        glm::vec3 vmin;
        /// maximal vertex of AABB
        glm::vec3 vmax;

};

#endif
