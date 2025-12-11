
#include "utils.h"

#include <string>

std::string mat4_to_string(const glm::mat4& matrix)
{
    constexpr int sideSize = 4;
    std::string res;
    res.resize(sideSize*(2*sideSize+1));

    for (int i = 0; i < sideSize; i++) {
        for (int j = 0; j < sideSize; j++) {
            res.push_back(matrix[i][j]);
            res.push_back(' ');
        }
        res.push_back('\n');
    }
    return res;
}
