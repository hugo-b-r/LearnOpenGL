#ifndef CAMERA_H
#define CAMERA_H

#include "Entity.h"
#include "Configuration.h"
#include "../deps/glm/glm/glm.hpp"
#include "../deps/glm/glm/gtc/matrix_transform.hpp"

class Camera : public Entity {
    public:
        Camera(const Configuration& c);
        void update();

        const glm::mat4& getViewMatrix() const;
        const glm::mat4& getProjMatrix() const;
        const glm::mat4& getProjectionViewMatrix() const;

        static glm::mat4 makeViewMatrix(const Camera &camera);
        static glm::mat4 makeModelMatrix(const Entity &entity);
    private:
        glm::mat4 m_projectionMatrix{1.0f};
        glm::mat4 m_viewMatrix{1.0f};
        glm::mat4 m_projViewMatrix{1.0f};

        Configuration m_config;
};

#endif // CAMERA_H
