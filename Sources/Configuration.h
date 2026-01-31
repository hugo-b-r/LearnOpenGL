#ifndef CONFIGURATION_H
#define CONFIGURATION_H

struct Configuration {
    int windowWidth = 1000;
    int windowHeight = 600;
    bool isFullScreen = false;
    int renderDistance = 8;
    int fov = 90;
    bool mouseFree = false;
    bool showDebugInfos = false;
    int camSpeedFactor = 5;
};

#endif
