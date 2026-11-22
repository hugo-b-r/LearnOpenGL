// a simple file to test the test system
//
#include <gtest/gtest.h>
#include <World/Chunk.h>

TEST(MyLibTest, TestExample) {
    EXPECT_EQ(1, 1); // Replace with actual test logic
}


TEST(MyLibTest, Test2) {
    EXPECT_EQ(2, 2);
}


TEST(Chunk, ChunkBlockIsInChunk) {

    Chunk c;
    EXPECT_EQ(c.containPos(glm::ivec3(5, 5, 5)), true);
}
