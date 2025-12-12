#ifndef CONFIGURATION_H
#define CONFIGURATION_H

struct Configuration {
    int windowWidth = 1300;
    int windowHeight = 800;
    bool isFullScreen = false;
    int renderDistance = 8;
    int fov = 90;
    bool mouseFree = false;
};

#endif
