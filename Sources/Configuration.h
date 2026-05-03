#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "Camera.h"

class Configuration {
      public:
        Configuration();
        ~Configuration();

        int windowWidth = 1000;
        int windowHeight = 600;
        bool isFullScreen = false;
        int renderDistance = 1;
        int fov = 90;
        bool mouseFree = false;
        bool showDebugInfos = false;
        int camSpeedFactor = 5;
        bool isFlying = true;
        bool checkCollisions = true;
        // to be called by Scene !!!
        bool getCheckCollisions();
        bool setCheckCollisions(bool val);
      private:
};

#endif
