#include "Frustum.h"

Frustum::Frustum() {};

Frustum::Frustum(const Frustum & f) {
    this->m_planes = f.m_planes;
}

Frustum::Frustum(glm::mat4 projViewMat) {
    updateFrom(projViewMat);
}

Frustum& Frustum::operator=(const Frustum& f) {
    this->m_planes = f.m_planes;
    return *this;
}


void Frustum::updateFrom(glm::mat4 projViewMat) {
    glm::mat4 vpt = glm::transpose(projViewMat);
    // using outsides normals
    m_planes = {
        // left, right, bottom, top
        {
            (vpt[3] + vpt[0]),
            (vpt[3] - vpt[0]),
            (vpt[3] + vpt[1]),
            (vpt[3] - vpt[1]),
            // near, far
            (vpt[3] + vpt[2]),
            (vpt[3] - vpt[2]),
        }
    };
}

bool Frustum::IsAabbVisible(AABB const & box )
{
    // check box outside/inside of frustum
    for( int i=0; i<6; i++ )
    {
        int out = 0;
        out += ((dot( m_planes[i], glm::vec4(box.vmin.x, box.vmin.y, box.vmin.z, 1.0f) ) < 0.0 )?1:0);
        out += ((dot( m_planes[i], glm::vec4(box.vmax.x, box.vmin.y, box.vmin.z, 1.0f) ) < 0.0 )?1:0);
        out += ((dot( m_planes[i], glm::vec4(box.vmin.x, box.vmax.y, box.vmin.z, 1.0f) ) < 0.0 )?1:0);
        out += ((dot( m_planes[i], glm::vec4(box.vmax.x, box.vmax.y, box.vmin.z, 1.0f) ) < 0.0 )?1:0);
        out += ((dot( m_planes[i], glm::vec4(box.vmin.x, box.vmin.y, box.vmax.z, 1.0f) ) < 0.0 )?1:0);
        out += ((dot( m_planes[i], glm::vec4(box.vmax.x, box.vmin.y, box.vmax.z, 1.0f) ) < 0.0 )?1:0);
        out += ((dot( m_planes[i], glm::vec4(box.vmin.x, box.vmax.y, box.vmax.z, 1.0f) ) < 0.0 )?1:0);
        out += ((dot( m_planes[i], glm::vec4(box.vmax.x, box.vmax.y, box.vmax.z, 1.0f) ) < 0.0 )?1:0);
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
