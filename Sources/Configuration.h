#ifndef CONFIGURATION_H
#define CONFIGURATION_H

struct Configuration {
    int windowWidth = 1300;
    int windowHeight = 800;
    bool isFullScreen = false;
    int renderDistance = 4;
    int fov = 90;
    bool mouseFree = false;
    bool showDebugInfos = false;
    int camSpeedFactor = 5;
};

#endif
