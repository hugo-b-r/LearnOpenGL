#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"
#include "Configuration.h"

class Scene {
    public:
        Scene() : m_config (), m_cam (m_config) {

        }

        Scene(Camera cam) : m_config(), m_cam( cam ) {

        }

        Camera& getCamera() {
            return m_cam;
        }
        Configuration& getConfig() {
            return m_config;
        }
    private:
        Configuration m_config;
        Camera m_cam;

};


#endif
