#pragma once

#include <vector>

#include <Vroom/Render/Abstraction/Texture2D.h>

#include <glm/glm.hpp>

class ScalarField
{
public:
    ScalarField();
    ScalarField(size_t width, size_t height);

    void setFromTexture(const vrm::Texture2D& texture);
    vrm::Texture2D toTexture(float min, float max) const;

    inline size_t index(size_t x, size_t y) const { return y * m_Width + x; }

    inline size_t getWidth() const { return m_Width; }
    inline size_t getHeight() const { return m_Height; }

private:
    size_t m_Width = 0;
    size_t m_Height = 0;
    std::vector<float> m_Data;
};