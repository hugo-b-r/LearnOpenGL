#ifndef CAMERA_H
#define CAMERA_H

#include "Entity.h"
#include "Configuration.h"
#include "Mathematics/AABB.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>



struct Frustum
{
    std::vector<glm::vec4> planes;

    bool IsAabbVisible(AABB const & box )
    {
        // check box outside/inside of frustum
        for( int i=0; i<6; i++ )
        {
            int out = 0;
            out += ((dot( planes[i], glm::vec4(box.vmin.x, box.vmin.y, box.vmin.z, 1.0f) ) < 0.0 )?1:0);
            out += ((dot( planes[i], glm::vec4(box.vmax.x, box.vmin.y, box.vmin.z, 1.0f) ) < 0.0 )?1:0);
            out += ((dot( planes[i], glm::vec4(box.vmin.x, box.vmax.y, box.vmin.z, 1.0f) ) < 0.0 )?1:0);
            out += ((dot( planes[i], glm::vec4(box.vmax.x, box.vmax.y, box.vmin.z, 1.0f) ) < 0.0 )?1:0);
            out += ((dot( planes[i], glm::vec4(box.vmin.x, box.vmin.y, box.vmax.z, 1.0f) ) < 0.0 )?1:0);
            out += ((dot( planes[i], glm::vec4(box.vmax.x, box.vmin.y, box.vmax.z, 1.0f) ) < 0.0 )?1:0);
            out += ((dot( planes[i], glm::vec4(box.vmin.x, box.vmax.y, box.vmax.z, 1.0f) ) < 0.0 )?1:0);
            out += ((dot( planes[i], glm::vec4(box.vmax.x, box.vmax.y, box.vmax.z, 1.0f) ) < 0.0 )?1:0);
            if( out==8 ) return false;
        }

        // // check frustum outside/inside box
        // int out;
        // out=0; for( int i=0; i<8; i++ ) out += ((fru.mPoints[i].x > box.vmax.x)?1:0); if( out==8 ) return false;
        // out=0; for( int i=0; i<8; i++ ) out += ((fru.mPoints[i].x < box.vmin.x)?1:0); if( out==8 ) return false;
        // out=0; for( int i=0; i<8; i++ ) out += ((fru.mPoints[i].y > box.vmax.y)?1:0); if( out==8 ) return false;
        // out=0; for( int i=0; i<8; i++ ) out += ((fru.mPoints[i].y < box.vmin.y)?1:0); if( out==8 ) return false;
        // out=0; for( int i=0; i<8; i++ ) out += ((fru.mPoints[i].z > box.vmax.z)?1:0); if( out==8 ) return false;
        // out=0; for( int i=0; i<8; i++ ) out += ((fru.mPoints[i].z < box.vmin.z)?1:0); if( out==8 ) return false;

        return true;
    }

    // bool IsAabbVisible(const AABB& box) {

    //     const glm::vec3& vmin = box.vmin;
    //     const glm::vec3& vmax = box.vmax;

    //     for (size_t i = 0; i < planes.size(); ++i) {
    //         const glm::vec4& g = planes[i];
    //         if ((glm::dot(g, glm::vec4(vmin.x, vmin.y, vmin.z, 1.0f)) < 0.0) &&
    //             (glm::dot(g, glm::vec4(vmax.x, vmin.y, vmin.z, 1.0f)) < 0.0) &&
    //             (glm::dot(g, glm::vec4(vmin.x, vmax.y, vmin.z, 1.0f)) < 0.0) &&
    //             (glm::dot(g, glm::vec4(vmax.x, vmax.y, vmin.z, 1.0f)) < 0.0) &&
    //             (glm::dot(g, glm::vec4(vmin.x, vmin.y, vmax.z, 1.0f)) < 0.0) &&
    //             (glm::dot(g, glm::vec4(vmax.x, vmin.y, vmax.z, 1.0f)) < 0.0) &&
    //             (glm::dot(g, glm::vec4(vmin.x, vmax.y, vmax.z, 1.0f)) < 0.0) &&
    //             (glm::dot(g, glm::vec4(vmax.x, vmax.y, vmax.z, 1.0f)) < 0.0))
    //         {
    //             // Not visible - all returned negative
    //             return false;
    //         }
    //     }

    //     // Potentially visible
    //     return true;
    // }

};

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

        void updateUpVector();
        void createFrustum();

        int getRenderDistance() const;
        Frustum& getFrustum();
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
        Frustum m_frustum;
};

#endif // CAMERA_H
