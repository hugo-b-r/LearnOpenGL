#include "Camera.h"
#include "Configuration.h"

#include <format>
#include <spdlog/spdlog.h>

Camera::Camera(Configuration& c) : m_config(c) {

    float x = (float)m_config.windowWidth;
    float y = (float)m_config.windowHeight;
    float fov = (float)m_config.fov;
    m_yaw = 0.;
    m_pitch = 0.;
    lastX = (float)c.windowWidth/2.f;
    lastY = (float)c.windowHeight/2.f;

    m_projectionMatrix = glm::perspective(glm::radians(fov), x / y, 0.1f, 2000.0f);
    position = glm::vec3(0., 0., -3.5);
    m_frontPos = position - glm::vec3(0.0f, 0.0f, 4.0f);
    m_upVector = glm::vec3(0.0f, 1.0f,  0.0f);

    m_viewMatrix = makeViewMatrix(*this);
    m_projViewMatrix = m_projectionMatrix * m_viewMatrix;
}

void Camera::translate(glm::vec3 trans) {
    position += trans;
    //m_viewMatrix = glm::translate(m_viewMatrix, trans);
    updateProjViewMatrix();
}

void Camera::translate_forward(float how_much) {
    glm::vec3 t = m_frontPos;
    t[1] = 0;
    position += glm::normalize(t) * how_much;
    updateProjViewMatrix();
}

void Camera::translate_right(float how_much) {
    position += glm::normalize(glm::cross(m_frontPos, m_upVector)) * how_much;
    updateProjViewMatrix();

}
void Camera::translate_left(float how_much) {
    position -= glm::normalize(glm::cross(m_frontPos, m_upVector)) * how_much;
    updateProjViewMatrix();
}


std::string Camera::debug_pos() {
    std::string res = std::format("Pos : x : {:0.2f}; y : {:0.2f}; z : {:0.2f}; ", position.x, position.y, position.z);
    return res;
}

std::string Camera::debug_direction() {
    std::string res = std::format("Direction : x : {:0.2f}; y : {:0.2f}; z : {:0.2f}; ", m_frontPos.x, m_frontPos.y, m_frontPos.z);
    return res;
}


void Camera::updateProjViewMatrix() {
    m_projectionMatrix = glm::perspective(glm::radians((float)m_config.fov), (float)m_config.windowWidth / (float)m_config.windowHeight, 0.1f, 2000.0f);
    m_viewMatrix = glm::lookAt(position, position + m_frontPos, m_upVector);
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


int Camera::getRenderDistance() const {
    return m_config.renderDistance;
}
