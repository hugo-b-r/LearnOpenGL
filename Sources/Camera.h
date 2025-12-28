#ifndef CAMERA_H
#define CAMERA_H

#include "Entity.h"
#include "Configuration.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>

class Camera : public Entity {
    public:
        Camera(Configuration& c);
        void update();

        void translate(glm::vec3 trans);
        void translate_forward(float how_much);
        void translate_right(float how_much);
        void translate_left(float how_much);

        std::string debug_pos();
        std::string debug_direction();


        void updateProjViewMatrix();

        const glm::mat4& getViewMatrix() const;
        const glm::mat4& getProjMatrix() const;
        const glm::mat4& getProjectionViewMatrix() const;

        static glm::mat4 makeViewMatrix(const Camera &camera);
        static glm::mat4 makeModelMatrix(const Entity &entity);

        int getRenderDistance() const;
        float m_pitch;
        float m_yaw;
        float lastX, lastY;
        glm::vec3 m_frontPos;
    private:
        glm::mat4 m_projectionMatrix{1.0f};
        glm::mat4 m_viewMatrix{1.0f};
        glm::mat4 m_projViewMatrix{1.0f};
        // the place the camera is pointing at
        glm::vec3 m_upVector;

        Configuration& m_config;
};

#endif // CAMERA_H
