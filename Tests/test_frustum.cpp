#include <gtest/gtest.h>


#include "Camera.h"
#include "Configuration.h"
#include <spdlog/spdlog.h>
#include <iostream>



TEST(TestFrustum, FrustumNumber) {
    std::cout << "Running the test right here !" << std::endl;
    Configuration conf;
    Camera cam(conf);
    cam.createFrustum();
    Frustum f = cam.getFrustum();
    std::string res = "";
    for (int i = 0; i < f.planes.size(); ++i) {
        res = "";
        for (int j= 0; j < 4; j++) {
            res += std::to_string(f.planes[i][j]);
            res += ", ";
        }
        std::cout << res << std::endl;
    }

    EXPECT_EQ(0, 1);

}
