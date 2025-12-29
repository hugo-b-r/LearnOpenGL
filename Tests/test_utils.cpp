// a simple file to test the test system
//
#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include "utils.h"

TEST(TestUtils, TestfromBlockToChunkCoordinates1) {
    glm::ivec3 p(0, 0, 0);

    EXPECT_EQ(glm::ivec2(0, 0), fromBlockToChunkCoordinates(p)); // Replace with actual test logic

}

TEST(TestUtils, TestfromBlockToChunkCoordinates2) {
    glm::ivec3 p(17, 25, 0);

    EXPECT_EQ(glm::ivec2(1, 0), fromBlockToChunkCoordinates(p)); // Replace with actual test logic
}

TEST(TestUtils, TestfromBlockToChunkCoordinates3) {
    glm::ivec3 p(125, 12, 16);

    EXPECT_EQ(glm::ivec2(7, 1), fromBlockToChunkCoordinates(p)); // Replace with actual test logic
}


TEST(TestUtils, TestfromChunkToBlockCoordinates1) {
    glm::ivec2 p(0, 0);

    EXPECT_EQ(glm::ivec3(0, 0, 0), fromChunkToBlockCoordinates(p, 0)); // Replace with actual test logic

}

TEST(TestUtils, TestfromChunkToBlockCoordinates2) {
    glm::ivec2 p(17, 25);

    EXPECT_EQ(glm::ivec3(17*16, 0, 25*16), fromChunkToBlockCoordinates(p, 0)); // Replace with actual test logic
}

TEST(TestUtils, TestfromChunkToBlockCoordinates3) {
    glm::ivec2 p(125, 16);

    EXPECT_EQ(glm::ivec3(125*16,16, 256), fromChunkToBlockCoordinates(p, 16)); // Replace with actual test logic
}





TEST(TestUtils, TestfromGlobalToInChunkCoordinates1) {
    glm::ivec3 p(0, 0, 0);

    EXPECT_EQ(glm::ivec3(0, 0, 0), fromGlobalToInChunkCoordinates(p)); // Replace with actual test logic

}

TEST(TestUtils, TestfromGlobalToInChunkCoordinates2) {
    glm::ivec3 p(17, 19, 25);

    EXPECT_EQ(glm::ivec3(1, 19, 9), fromGlobalToInChunkCoordinates(p)); // Replace with actual test logic
}

TEST(TestUtils, TestfromGlobalToInChunkCoordinates3) {
    glm::ivec3 p(125, 15, 16);

    EXPECT_EQ(glm::ivec3(13,15, 0), fromGlobalToInChunkCoordinates(p)); // Replace with actual test logic
}

glm::ivec3 fromGlobalToInChunkCoordinates(glm::ivec3 blockPos);
