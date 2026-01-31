#include <gtest/gtest.h>


#include "Camera.h"
#include "Configuration.h"
#include "World/Chunk.h"
#include "World/WorldGenerator.h"
#include "glm/fwd.hpp"
#include <spdlog/spdlog.h>
#include <iostream>



TEST(TestFrustum, ChunkAABB) {
    Chunk c (glm::ivec2(0,-1));
    WorldGenerator wg;
    wg.buildChunk(c);
    
    Configuration conf;
    Camera cam(conf);
    cam.createFrustum();
    Frustum f = cam.getFrustum();
    
    
    EXPECT_EQ(f.IsAabbVisible(bound), true);

    
}



TEST(TestFrustum, FrustumNumber) {
    std::cout << "Running the test right here !" << std::endl;
    Configuration conf;
    Camera cam(conf);
    cam.createFrustum();
    Frustum f = cam.getFrustum();
    AABB bound = { glm::vec3(16, 0, -31), glm::vec3(31, 255, 16) };
    
    EXPECT_EQ(f.IsAabbVisible(bound), true);

    
}
