#ifndef CAMERA_H
#define CAMERA_H

#include "Entity.h"
#include "Mathematics/Frustum.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>

class Game;

class Camera : public Entity {
      public:
        Camera(Game &s);
        void update();

        void translate(glm::vec3 trans);
        void translate_forward(float how_much);
        void translate_right(float how_much);
        void translate_left(float how_much);

        std::string debug_pos();
        std::string debug_direction();

        void updateProjViewMatrix();

        const glm::mat4 &getViewMatrix() const;
        const glm::mat4 &getProjMatrix() const;
        const glm::mat4 &getProjectionViewMatrix() const;

        static glm::mat4 makeViewMatrix(const Camera &camera);
        static glm::mat4 makeModelMatrix(const Entity &entity);

        void updateUpVector();
        void createFrustum();

        void updateTranslateFunc(bool checkCol);

        int getRenderDistance() const;
        Frustum &getFrustum();
        float m_pitch;
        float m_yaw;
        float lastX, lastY;
        glm::vec3 m_frontPos;

      private:
        void updateAabb();
        void translate_with_collisions(glm::vec3 trans);
        void translate_no_collisions(glm::vec3 trans);

        glm::mat4 m_projectionMatrix{1.0f};
        glm::mat4 m_viewMatrix{1.0f};
        glm::mat4 m_projViewMatrix{1.0f};
        // the place the camera is pointing at
        glm::vec3 m_upVector;

        Game &m_scene;
        Frustum m_frustum;
        AABB m_boundingBox;

        void (Camera::*translate_func)(const glm::vec3 trans);
};

#endif // CAMERA_H
