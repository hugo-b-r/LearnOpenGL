#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>

/// A 3d world thing present in the space
class Entity {
    public:
        /// Constructor with default values
        Entity();
        /// Default constructor
        Entity(const glm::vec3& pos, const glm::vec3& rot);

        /// Returns position of the entity in a chunk
        glm::ivec2 chunkPosition() const;
        /// The entity's position
        glm::vec3 position;
        /// The entity's rotation
        glm::vec3 rotation;
        /// The entity's velocity
        glm::vec3 velocity;

    protected:
};

#endif
