#ifndef CAMERA_H
#define CAMERA_H

#include "Entity.h"
#include "Mathematics/Frustum.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>

class Game;

/// A camera representation in our virtual world
///
/// This is where the `player` is, and where it looks at...
class Camera : public Entity {
      public:
        /// Basic constructor
        /// \param s Reference to the main Game
        Camera(Game &s);

        /// update matrices for the representation of the camera
        void update();

        /// Move camera without rotating
        /// \param trans vector for the translation, so that  final_pos =
        /// current_pos + trans.
        void translate(glm::vec3 trans);

        /// A simplified version of #translate but in one direction, forward,
        /// with some distance.
        ///
        /// \param how_much how much distance you want to move camera forward
        void translate_forward(float how_much);

        /// A simplified version of #translate but in one direction,
        /// orthogonally right, horizontally from current direction
        /// \param how_much how much distance you want to move your camera
        /// horizontally to the right
        void translate_right(float how_much);

        /// A simplified version of #translate but in one direction,
        /// orthogonally left, horizontally, from current direction
        /// \param how_much how much distance you want to move your camera
        /// horizontally to the left
        void translate_left(float how_much);

        /// String representing current pos, targeted at showing debug infos
        ///
        /// It returns a string composed of x, y, and z parameters of the
        /// position/translation matrix
        std::string debug_pos();
        /// String representing current direction, targeted at showing debug
        /// infos
        ///
        /// It returns a string composed of x, y, and z parameters of the
        /// rotation matrix
        std::string debug_direction();

        /// Updating the projection, view and projection view
        void updateProjViewMatrix();

        /// Returns view Matrix
        const glm::mat4 &getViewMatrix() const;
        /// Returns projection Matrix
        const glm::mat4 &getProjMatrix() const;
        /// Returns the ProjectionView Matrix, that is the product of the
        /// projection and view matrix
        const glm::mat4 &getProjectionViewMatrix() const;


        /// Create the view matrix out of another camera
        static glm::mat4 makeViewMatrix(const Camera &camera);

        /// Create the model matrix out of another camera
        static glm::mat4 makeModelMatrix(const Entity &entity);

        /// Update the value of the vector showing the up of the camera
        void updateUpVector();
        /// Create a frustum for the camera
        void createFrustum();

        /// Update whether we translate using collisions or not
        ///
        /// \param checkCol whether checking collisions or not
        void updateTranslateFunc(bool checkCol);

        /// return render distance
        int getRenderDistance() const;

        /// Getter for the frustum
        Frustum &getFrustum();

        /// pitch parameter for camera
        float m_pitch;
        /// yaw paramerted
        float m_yaw;
        /// last coordinates from camera, before this update
        float lastX;
        /// last coordinates from camera, before this update
        float lastY;
        /// where is the screen of the camera
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
