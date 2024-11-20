#pragma once

#include <vector>

#include <Vroom/Render/Abstraction/Texture2D.h>

#include <glm/glm.hpp>

class ScalarField
{
public:
    inline constexpr ScalarField() {}
    inline constexpr ScalarField(size_t width, size_t height) : m_Width(width), m_Height(height), m_Data(width * height, 0.5f) {}

    void setFromTexture(const vrm::Texture2D& texture);
    vrm::Texture2D toTexture(float min, float max) const;

    inline constexpr glm::vec2 getGradient(size_t x, size_t y) const
    {
        glm::vec2 grad(0.f);

        if (x > 0 && x < m_Width - 1)
        {
            grad.x = (m_Data[index(x + 1, y)] - m_Data[index(x - 1, y)]) / 2.0f;
        }
        else if (x == 0)
        {
            grad.x = m_Data[index(x + 1, y)] - m_Data[index(x, y)];
        }
        else
        {
            grad.x = m_Data[index(x, y)] - m_Data[index(x - 1, y)];
        }

        if (y > 0 && y < m_Height - 1)
        {
            grad.y = (m_Data[index(x, y + 1)] - m_Data[index(x, y - 1)]) / 2.0f;
        }
        else if (y == 0)
        {
            grad.y = m_Data[index(x, y + 1)] - m_Data[index(x, y)];
        }
        else
        {
            grad.y = m_Data[index(x, y)] - m_Data[index(x, y - 1)];
        }

        return grad;
    }

    inline constexpr size_t index(size_t x, size_t y) const { return y * m_Width + x; }

    inline constexpr float getValue(size_t i) const { return m_Data[i]; }
    inline constexpr float getValue(size_t x, size_t y) const { return getValue(index(x, y)); }

    inline constexpr size_t getWidth() const { return m_Width; }
    inline constexpr size_t getHeight() const { return m_Height; }

    inline constexpr void setValue(size_t i, float value) { m_Data[i] = value; }
    inline constexpr void setValue(size_t x, size_t y, float value) { setValue(index(x, y), value); }

private:
    size_t m_Width = 0;
    size_t m_Height = 0;
    std::vector<float> m_Data;
};