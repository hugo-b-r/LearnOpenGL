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

        void translate(glm::vec3 trans);
        void translate_forward(float how_much);
        void translate_right(float how_much);
        void translate_left(float how_much);


        void updateProjViewMatrix();

        const glm::mat4& getViewMatrix() const;
        const glm::mat4& getProjMatrix() const;
        const glm::mat4& getProjectionViewMatrix() const;

        static glm::mat4 makeViewMatrix(const Camera &camera);
        static glm::mat4 makeModelMatrix(const Entity &entity);
        float m_pitch;
        float m_yaw;
        float lastX, lastY;
        glm::vec3 m_frontPos;
    private:
        glm::mat4 m_projectionMatrix{1.0f};
        glm::mat4 m_viewMatrix{1.0f};
        glm::mat4 m_projViewMatrix{1.0f};
        // the place the camera is pointing at
        glm::vec3 m_target;
        glm::vec3 m_direction;
        glm::vec3 m_upVector;




        Configuration m_config;
};

#endif // CAMERA_H
