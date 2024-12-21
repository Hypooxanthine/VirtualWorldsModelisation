#include "ScalarFields/HeightField.h"

#include <queue>
#include <cmath>
#include <Vroom/Core/Log.h>

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

vrm::MeshData HeightField::toMeshData() const
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
                    static_cast<float>(x) * getPointSpacing(),
                    getHeight(x, y),
                    static_cast<float>(y) * getPointSpacing()
                ),
                glm::vec3(0.f, 1.f, 0.f)
            });
        }
    }

    for (size_t y = 0; y < getSizeY()- 1; ++y)
    {
        for (size_t x = 0; x < getSizeX() - 1; ++x)
        {
            indices.push_back(static_cast<uint32_t>(index(x, y)));
            indices.push_back(static_cast<uint32_t>(index(x + 1, y + 1)));
            indices.push_back(static_cast<uint32_t>(index(x + 1, y)));

            indices.push_back(static_cast<uint32_t>(index(x, y)));
            indices.push_back(static_cast<uint32_t>(index(x, y + 1)));
            indices.push_back(static_cast<uint32_t>(index(x + 1, y + 1)));
            

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

ScalarField HeightField::getDrainageAreaScalarField(float power) const
{
    ScalarField drainageArea(getSizeX(), getSizeY(), 1.f);

    using Pair = std::pair<float, size_t>;

    std::priority_queue<Pair, std::vector<Pair>, std::less<Pair>> queue;

    for (size_t i = 0; i < getSizeX() * getSizeY(); ++i)
    {
        queue.push({ getHeight(i), i });
    }

    while (!queue.empty())
    {
        auto [currentValue, currentIndex] = queue.top();
        queue.pop();
        //VRM_LOG_TRACE("Current value: {}, index: {}", currentValue, currentIndex);

        /**
         * v00 v01 v02
         * v10 v11 v12
         * v20 v21 v22
         */
        int x1 = static_cast<int>(xCoord(currentIndex));
        int y1 = static_cast<int>(yCoord(currentIndex));
        int x0 = x1 > 0 ? x1 - 1 : x1;
        int x2 = x1 < getSizeX() - 1 ? x1 + 1 : x1;
        int y0 = y1 > 0 ? y1 - 1 : y1;
        int y2 = y1 < getSizeY() - 1 ? y1 + 1 : y1;

        float totalSlope = 0.f;
        std::vector<float> slopes((x2 - x0 + 1) * (y2 - y0 + 1), 0.f);

        for (int x = x0; x <= x2; ++x)
        {
            for (int y = y0; y <= y2; ++y)
            {
                if (x == x1 && y == y1)
                    continue;

                float delta = currentValue - getHeight(x, y);

                if (delta <= 0.f)
                    continue;
                
                float slope = delta / getPointSpacing();

                if (std::abs(x - x0 + y - y0) == 2)
                    slope /= std::sqrt(2.f); // Diagonal

                slope = std::pow(slope, power);
                
                totalSlope += slope;
                slopes[(x - x0) + (y - y0) * (x2 - x0 + 1)] = slope;
            }
        }

        for (int x = x0; x <= x2; ++x)
        {
            for (int y = y0; y <= y2; ++y)
            {
                float slope = slopes[(x - x0) + (y - y0) * (x2 - x0 + 1)];

                if (slope == 0.f)
                    continue;

                float flow = currentValue * slope / totalSlope;
                float newValue = drainageArea.getValue(x, y) + flow;
                drainageArea.setValue(x, y, newValue);
            }
        }
    }

    return drainageArea;
}

ScalarField HeightField::getStreamPowerScalarField(float power) const
{
    ScalarField drainageArea = getDrainageAreaScalarField(power);
    ScalarField slope = getSlopeScalarField();
    ScalarField streamPower(getSizeX(), getSizeY(), 1.f);

    for (size_t i = 0; i < getSizeX() * getSizeY(); ++i)
    {
        streamPower.setValue(i, drainageArea.getValue(i) * std::sqrtf(slope.getValue(i)));
    }

    return streamPower;
}

ScalarField HeightField::getWetnessIndexScalarField(float power) const
{
    ScalarField drainageArea = getDrainageAreaScalarField(power);
    ScalarField slope = getSlopeScalarField();
    ScalarField wetnessIndex(getSizeX(), getSizeY(), 1.f);

    for (size_t i = 0; i < getSizeX() * getSizeY(); ++i)
    {
        wetnessIndex.setValue(i, std::log(drainageArea.getValue(i)) / (slope.getValue(i) + 1.f));
    }

    return wetnessIndex;
}