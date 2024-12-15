#pragma once

#include <glm/glm.hpp>
#include <string>

namespace Ovis
{
    template <typename T, glm::length_t R, glm::length_t C>
    std::string MatrixToString(const glm::mat<R, C, T>& matrix)
    {
        std::ostringstream oss;
        oss << "[\n";
        for (glm::length_t row = 0; row < R; ++row)
        {
            oss << "  ";
            for (glm::length_t col = 0; col < C; ++col)
            {
                oss << matrix[row][col];
                if (col < C - 1)
                    oss << ", ";
            }
            oss << (row < R - 1 ? ",\n" : "\n");
        }
        oss << "]";
        return oss.str();
    }
}