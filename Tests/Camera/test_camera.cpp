#include <gtest/gtest.h>
#include "../../Sources/Camera.h"
#include "../../Sources/Configuration.h"


TEST(Camera, TestCreation) {
    Configuration c;
    Camera cam(c);
}
