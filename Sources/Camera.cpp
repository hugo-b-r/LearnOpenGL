#include "Camera.h"
#include "Configuration.h"
#include "CubeTexture.h"
#include "Scene.h"
#include "World/WorldGenerator.h"
#include "glm/common.hpp"

#include "Mathematics/Noise.h"

#include <format>
#include <spdlog/spdlog.h>

const glm::vec3 AABB_DIMS = glm::vec3(0.8, 1.8, 0.8);

const glm::vec3 initial_pos = glm::vec3(0.0, 4.0, 0.0);

Camera::Camera(Game &s) : m_scene(s) {

        m_yaw = 0.;
        m_pitch = 0.;
        lastX = (float)s.configuration.windowWidth / 2.f;
        lastY = (float)s.configuration.windowHeight / 2.f;

        position = glm::vec3(initial_pos.x, WorldGenerator::soilHeightAt(glm::vec2(initial_pos.x, initial_pos.z)) + initial_pos.y, initial_pos.z);

        m_frontPos = position - glm::vec3(0.0f, 0.0f, 4.0f);
        m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);

        updateTranslateFunc(s.configuration.getCheckCollisions());

        updateProjViewMatrix();

        createFrustum();
        updateAabb();
}

void Camera::update() {
        updateProjViewMatrix();
        updateAabb();
        updateUpVector();
        createFrustum();
        if (!m_scene.configuration.isFlying) {
                // get down
        }
}

void Camera::translate(glm::vec3 trans) {
        // calls the correct function used by parameters
        (this->*translate_func)(trans);

}
void Camera::updateTranslateFunc(bool checkCol) {
        if (checkCol) {
                translate_func = &Camera::translate_with_collisions;
        } else {
                translate_func = &Camera::translate_no_collisions;
        }
}

void Camera::translate_with_collisions(glm::vec3 trans) {
        // for now, just translate without collision detection
        glm::vec3 aabb_half_dim = AABB_DIMS / 2.f;

        glm::vec3 new_position = position + trans;

        // delta is trans
        for (int i = 0; i < 3; ++i) {
                glm::vec3 target_position = new_position;

                target_position[i] += trans[i];

                glm::vec3 min = glm::floor(target_position - aabb_half_dim);
                glm::vec3 max = glm::ceil(target_position - aabb_half_dim);

                bool collided = false;
                for (int x = min.x; x < max.x; ++x) {
                        for (int y = min.y; y < max.y; ++y) {
                                for (int z = min.z; z < max.z; ++z) {
                                        glm::vec3 tmp_pos(x, y, z);
                                        std::optional<CubeAtlas> block =
                                            m_scene.world.getBlockAt(
                                                tmp_pos);
                                        if (block.has_value() && block.value() != CubeAtlas::Air) {
                                                collided = true;
                                                break;
                                        }
                                }
                                if (collided) {
                                        break;
                                }
                        }
                        if (collided) {
                                break;
                        }
                }
                if (!collided) {
                        position[i] = target_position[i];
                }
        }
        // m_viewMatrix = glm::translate(m_viewMatrix, trans);
        update();
}

void Camera::translate_no_collisions(glm::vec3 trans) {
        // for now, just translate without collision detection
        position += trans;
        update();
}

void Camera::translate_forward(float how_much) {
        glm::vec3 t = m_frontPos;
        t[1] = 0;
        translate(glm::normalize(t) * how_much);
        update();
}

void Camera::translate_right(float how_much) {
        translate(glm::normalize(glm::cross(m_frontPos, glm::vec3(0, 1, 0))) *
                  how_much);
        update();
}
void Camera::translate_left(float how_much) {
        translate(-glm::normalize(glm::cross(m_frontPos, glm::vec3(0, 1, 0))) *
                  how_much);
        update();
}

std::string Camera::debug_pos() {
        std::string res =
            std::format("Pos : x : {:0.2f}; y : {:0.2f}; z : {:0.2f}; ",
                        position.x, position.y, position.z);
        return res;
}

std::string Camera::debug_direction() {
        std::string res =
            std::format("Direction : x : {:0.2f}; y : {:0.2f}; z : {:0.2f}; ",
                        m_frontPos.x, m_frontPos.y, m_frontPos.z);
        return res;
}

void Camera::updateProjViewMatrix() {
        m_projectionMatrix =
            glm::perspective(glm::radians((float)m_scene.configuration.fov),
                             (float)m_scene.configuration.windowWidth /
                                 (float)m_scene.configuration.windowHeight,
                             0.1f, 2000.0f);
        m_viewMatrix =
            glm::lookAt(position, position + m_frontPos, glm::vec3(0, 1, 0));
        m_projViewMatrix = m_projectionMatrix * m_viewMatrix;
}

const glm::mat4 &Camera::getProjMatrix() const { return m_projectionMatrix; }

const glm::mat4 &Camera::getProjectionViewMatrix() const {
        return m_projViewMatrix;
}

const glm::mat4 &Camera::getViewMatrix() const { return m_viewMatrix; }

glm::mat4 Camera::makeModelMatrix(const Entity &entity) {
        glm::mat4 matrix;

        matrix =
            glm::rotate(matrix, glm::radians(entity.rotation.x), {1, 0, 0});
        matrix =
            glm::rotate(matrix, glm::radians(entity.rotation.y), {0, 1, 0});
        matrix =
            glm::rotate(matrix, glm::radians(entity.rotation.z), {0, 0, 1});

        matrix = glm::translate(matrix, entity.position);

        return matrix;
}

glm::mat4 Camera::makeViewMatrix(const Camera &camera) {
        glm::mat4 matrix(1.f);

        matrix =
            glm::rotate(matrix, glm::radians(camera.rotation.x), {1, 0, 0});
        matrix =
            glm::rotate(matrix, glm::radians(camera.rotation.y), {0, 1, 0});
        matrix =
            glm::rotate(matrix, glm::radians(camera.rotation.z), {0, 0, 1});

        matrix = glm::translate(matrix, -camera.position);

        return matrix;
}

int Camera::getRenderDistance() const {
        return m_scene.configuration.renderDistance;
}

void Camera::updateUpVector() {
        glm::mat4 matrix;
        matrix = glm::rotate(matrix, glm::radians(rotation.x), {1, 0, 0});
        matrix = glm::rotate(matrix, glm::radians(rotation.y), {0, 1, 0});
        matrix = glm::rotate(matrix, glm::radians(rotation.z), {0, 0, 1});
        m_upVector = glm::vec3(glm::vec4(0, 1, 0, 1) * matrix);
}

void Camera::createFrustum() { m_frustum.updateFrom(m_projViewMatrix); }

Frustum &Camera::getFrustum() { return m_frustum; }

void Camera::updateAabb() {
        // AABB:
        //  w : 0.7, h : 1.8, l : 0.7
        m_boundingBox.getVmin() = position - glm::vec3(0.35, 1.65, 0.35);
        m_boundingBox.getVmax() = position + glm::vec3(0.35, 0.15, 0.35);
}
