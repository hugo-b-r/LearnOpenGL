#include "Configuration.h"
#include "Camera.h"

Configuration::Configuration() {}

Configuration::~Configuration() {}

bool Configuration::getCheckCollisions() { return checkCollisions; }

bool Configuration::setCheckCollisions(bool val) {
        checkCollisions = val;
        return checkCollisions;
}
