#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"
#include "Configuration.h"

class Scene {
    public:
        Scene(Configuration c) : m_config (c), m_cam (c) {

        }

        // avec une camera précédement initialisée avec la configuration
        Scene(Configuration c, Camera cam) : m_config(c), m_cam( cam ) {

        }

        Scene() : m_config(), m_cam(m_config) {

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
