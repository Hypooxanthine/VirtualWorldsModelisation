#include "ScalarFields/HeightField.h"

float HeightField::getAverageSlope(size_t x, size_t y) const
{
    float slope = 0.f;
    size_t count = 0;

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (x + i >= 0 && x + i < getSizeX() && y + j >= 0 && y + j < getSizeY())
            {
                slope += getSlope(x + i, y + j);
                count++;
            }
        }
    }

    return slope / count;
}

vrm::MeshData HeightField::toMeshData(float scale) const
{
    std::vector<vrm::Vertex> vertices;
    std::vector<uint32_t> indices;

    for (size_t y = 0; y < getSizeY(); ++y)
    {
        for (size_t x = 0; x < getSizeX(); ++x)
        {
            vertices.push_back({
                glm::vec3(
                    static_cast<float>(x) / getSizeX() - .5f,
                    getHeight(x, y),
                    static_cast<float>(y) / getSizeY() - .5f
                ) * scale,
                glm::vec3(0.f, 1.f, 0.f)
            });
        }
    }

    for (size_t y = 0; y < getSizeY()- 1; ++y)
    {
        for (size_t x = 0; x < getSizeX() - 1; ++x)
        {
            indices.push_back(index(x, y));
            indices.push_back(index(x + 1, y + 1));
            indices.push_back(index(x + 1, y));

            indices.push_back(index(x, y));
            indices.push_back(index(x, y + 1));
            indices.push_back(index(x + 1, y + 1));
        }
    }

    return { vertices, indices };
}
