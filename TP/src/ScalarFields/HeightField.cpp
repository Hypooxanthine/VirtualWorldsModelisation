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
    std::vector<glm::vec3> normals;

    float sizeX = static_cast<float>(getSizeX());
    float sizeY = static_cast<float>(getSizeY());
    float minSize = std::min(sizeX, sizeY);

    for (size_t y = 0; y < getSizeY(); ++y)
    {
        for (size_t x = 0; x < getSizeX(); ++x)
        {
            vertices.push_back({
                glm::vec3(
                    (static_cast<float>(x) - sizeX / 2.f),
                    getHeight(x, y),
                    (static_cast<float>(y) - sizeY / 2.f)
                ) * scale / minSize,
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
            

            glm::vec3 v0 = vertices[indices[indices.size() - 6]].position;
            glm::vec3 v1 = vertices[indices[indices.size() - 5]].position;
            glm::vec3 v2 = vertices[indices[indices.size() - 4]].position;
            glm::vec3 v3 = vertices[indices[indices.size() - 3]].position;

            glm::vec3 normal0 = glm::normalize(glm::cross(v1 - v0, v2 - v0));
            glm::vec3 normal1 = glm::normalize(glm::cross(v2 - v0, v3 - v0));

            normals.push_back(normal0);
            normals.push_back(normal0);
            normals.push_back(normal0);

            normals.push_back(normal1);
            normals.push_back(normal1);
            normals.push_back(normal1);
        }
    }

    return { vertices, indices };
}
