// a simple file to test the test system
//
#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>
#include "utils.h"

TEST(TestUtils, TestfromBlockToChunkCoordinates1) {
    glm::ivec3 p(0, 0, 0);

    EXPECT_EQ(glm::ivec2(0, 0), fromBlockToChunkCoordinates(p));

}

TEST(TestUtils, TestfromBlockToChunkCoordinates2) {
    glm::ivec3 p(17, 25, 0);

    EXPECT_EQ(glm::ivec2(1, 0), fromBlockToChunkCoordinates(p));
}

TEST(TestUtils, TestfromBlockToChunkCoordinates3) {
    glm::ivec3 p(125, 12, 16);

    EXPECT_EQ(glm::ivec2(7, 1), fromBlockToChunkCoordinates(p));
}


TEST(TestUtils, TestfromBlockToChunkCoordinates4) {
    glm::ivec3 p(-16, 12, -32);

    EXPECT_EQ(glm::ivec2(-1, -2), fromBlockToChunkCoordinates(p));
}


TEST(TestUtils, TestfromBlockToChunkCoordinates5) {
    glm::ivec3 p(-16, 12, -33);

    EXPECT_EQ(glm::ivec2(-1, -3), fromBlockToChunkCoordinates(p));
}




TEST(TestUtils, TestfromChunkToBlockCoordinates1) {
    glm::ivec2 p(0, 0);

    EXPECT_EQ(glm::ivec3(0, 0, 0), fromChunkToBlockCoordinates(p, 0));

}

TEST(TestUtils, TestfromChunkToBlockCoordinates2) {
    glm::ivec2 p(17, 25);

    EXPECT_EQ(glm::ivec3(17*16, 0, 25*16), fromChunkToBlockCoordinates(p, 0));
}

TEST(TestUtils, TestfromChunkToBlockCoordinates3) {
    glm::ivec2 p(125, 16);

    EXPECT_EQ(glm::ivec3(125*16,16, 256), fromChunkToBlockCoordinates(p, 16));
}






TEST(TestUtils, TestfromGlobalToInChunkCoordinates1) {
    glm::ivec3 p(0, 0, 0);

    EXPECT_EQ(glm::ivec3(0, 0, 0), fromGlobalToInChunkCoordinates(p));

}

TEST(TestUtils, TestfromGlobalToInChunkCoordinates2) {
    glm::ivec3 p(17, 19, 25);

    EXPECT_EQ(glm::ivec3(1, 19, 9), fromGlobalToInChunkCoordinates(p));
}

TEST(TestUtils, TestfromGlobalToInChunkCoordinates3) {
    glm::ivec3 p(125, 15, 16);

    EXPECT_EQ(glm::ivec3(13,15, 0), fromGlobalToInChunkCoordinates(p));
}

TEST(TestUtils, TestfromGlobalToInChunkCoordinates4) {
    glm::ivec3 p(-16, 0, -32);

    EXPECT_EQ(glm::ivec3(0,0, 0), fromGlobalToInChunkCoordinates(p));
}

TEST(TestUtils, TestfromGlobalToInChunkCoordinates5) {
    glm::ivec3 p(-17, 0, -33);
    spdlog::info("func : {} {} {}", fromGlobalToInChunkCoordinates(p).x, fromGlobalToInChunkCoordinates(p).y, fromGlobalToInChunkCoordinates(p).z);
    EXPECT_EQ(glm::ivec3(15,0, 15), fromGlobalToInChunkCoordinates(p));
}


TEST(TestUtils, TestfromInChunkToGlobalCoordinates1) {
    glm::ivec3 p(0, 0, 0);
    glm::ivec2 chunkPos(0, 0);

    EXPECT_EQ(glm::ivec3(0, 0, 0), fromInChunkToGlobalCoordinates(p, chunkPos));

}

TEST(TestUtils, TestfromInChunkToGlobalCoordinates2) {
    glm::ivec3 p(0, 0, 0);
    glm::ivec2 chunkPos(1, 1);

    EXPECT_EQ(glm::ivec3(16, 0, 16), fromInChunkToGlobalCoordinates(p, chunkPos));
}

TEST(TestUtils, TestfromInChunkToGlobalCoordinates3) {
    glm::ivec3 p(12, 15, 4);
    glm::ivec2 chunkPos(42, 420);

    EXPECT_EQ(glm::ivec3(684,15, 6724), fromInChunkToGlobalCoordinates(p, chunkPos));
}


TEST(TestUtils, TestfromInChunkToGlobalCoordinates4) {
    glm::ivec3 p(12, 0, 13);
    glm::ivec2 chunkPos(-1, -2);

    EXPECT_EQ(glm::ivec3(-4, 0, -19), fromInChunkToGlobalCoordinates(p, chunkPos));

}
