#include "Camera.h"
#include "Configuration.h"

Camera::Camera(const Configuration& c) : m_config(c) {
    float x = (float)m_config.windowWidth;
    float y = (float)m_config.windowHeight;
    float fov = (float)m_config.fov;

    m_projectionMatrix = glm::perspective(glm::radians(fov), x / y, 0.1f, 2000.0f);
    position = glm::vec3(0., 0., -3.5);

    m_viewMatrix = makeViewMatrix(*this);
    m_projViewMatrix = m_projectionMatrix * m_viewMatrix;
}

const glm::mat4& Camera::getProjMatrix() const {
    return m_projectionMatrix;
}


const glm::mat4& Camera::getProjectionViewMatrix() const {
    return m_projViewMatrix;
}

const glm::mat4& Camera::getViewMatrix() const {
    return m_viewMatrix;
}

glm::mat4 Camera::makeModelMatrix(const Entity &entity)
{
    glm::mat4 matrix;

    matrix = glm::rotate(matrix, glm::radians(entity.rotation.x), {1, 0, 0});
    matrix = glm::rotate(matrix, glm::radians(entity.rotation.y), {0, 1, 0});
    matrix = glm::rotate(matrix, glm::radians(entity.rotation.z), {0, 0, 1});

    matrix = glm::translate(matrix, entity.position);

    return matrix;
}

glm::mat4 Camera::makeViewMatrix(const Camera &camera)
{
    glm::mat4 matrix(1.f);

    matrix = glm::rotate(matrix, glm::radians(camera.rotation.x), {1, 0, 0});
    matrix = glm::rotate(matrix, glm::radians(camera.rotation.y), {0, 1, 0});
    matrix = glm::rotate(matrix, glm::radians(camera.rotation.z), {0, 0, 1});

    matrix = glm::translate(matrix, -camera.position);

    return matrix;
}
