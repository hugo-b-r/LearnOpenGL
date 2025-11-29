#include "Entity.h"

Entity::Entity() : position(glm::vec3(0.f)), rotation(glm::vec3(0.f)), velocity(glm::vec3(0.f)) {}

Entity::Entity(const glm::vec3& pos, const glm::vec3& rot) : position(pos), rotation(rot), velocity(glm::vec3(0.f)) {}
