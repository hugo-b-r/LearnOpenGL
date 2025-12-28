#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>

class Entity {
    public:
        Entity();
        Entity(const glm::vec3& pos, const glm::vec3& rot);

        glm::ivec2 chunkPosition() const;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 velocity;

    protected:
};

#endif
