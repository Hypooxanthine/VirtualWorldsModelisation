#pragma once

#include <vector>
#include <array>

#include <Vroom/Render/Abstraction/Texture2D.h>
#include <Vroom/Asset/AssetData/MeshData.h>

#include <glm/glm.hpp>

class ScalarField
{
public:
    struct FromTextureSpecs
    {
        float pointSpacing = 1.f;
        union
        {
            std::array<float, 2> zMinMax = { 0.f, 1.f };

            struct
            {
                float zmin;
                float zmax;
            };
        };
    };
public:
    inline constexpr ScalarField() {}
    inline constexpr ScalarField(size_t width, size_t height) : m_Width(width), m_Height(height), m_Data(width * height, 0.f) {}

    void setFromTexture(const vrm::Texture2D& texture, const FromTextureSpecs& specs);
    vrm::Texture2D toTexture(float min, float max) const;
    vrm::Texture2D toTexture() const { return toTexture(getMin(), getMax()); }

    inline constexpr glm::vec2 getGradient(size_t x, size_t y) const;
    inline constexpr glm::vec2 getGradient(size_t i) const { return getGradient(xCoord(i), yCoord(i)); }

    inline float getGradientNorm(size_t x, size_t y) const { return glm::length(getGradient(x, y)); }
    inline float getGradientNorm(size_t i) const { return getGradientNorm(xCoord(i), yCoord(i)); }
    inline constexpr ScalarField getGradientNormScalarField() const;

    inline constexpr float getLaplacian(size_t x, size_t y) const;
    inline constexpr float getLaplacian(size_t i) const { return getLaplacian(xCoord(i), yCoord(i)); }
    inline constexpr ScalarField getLaplacianScalarField() const;

    inline constexpr bool isValidIndex(size_t i) const { return i < m_Data.size(); }
    inline constexpr bool isValidIndex(size_t x, size_t y) const { return x < m_Width && y < m_Height; }

    inline constexpr size_t index(size_t x, size_t y) const { return y * m_Width + x; }
    inline constexpr size_t xCoord(size_t i) const { return i % m_Width; }
    inline constexpr size_t yCoord(size_t i) const { return i / m_Width; }

    inline constexpr float getValue(size_t i) const { return m_Data[i]; }
    inline constexpr float getValue(size_t x, size_t y) const { return getValue(index(x, y)); }

    inline constexpr float getMin() const;
    inline constexpr float getMax() const;

    inline constexpr size_t getWidth() const { return m_Width; }
    inline constexpr size_t getHeight() const { return m_Height; }
    inline constexpr float getPointSpacing() const { return m_PointSpacing; }

    inline constexpr void setValue(size_t i, float value) { m_Data[i] = value; }
    inline constexpr void setValue(size_t x, size_t y, float value) { setValue(index(x, y), value); }

private:
    size_t m_Width = 0;
    size_t m_Height = 0;
    std::vector<float> m_Data;

    float m_PointSpacing = 1.f;
};

inline constexpr glm::vec2 ScalarField::getGradient(size_t x, size_t y) const
{
    glm::vec2 grad(0.f);

    if (x > 0 && x < m_Width - 1)
        grad.x = (m_Data[index(x + 1, y)] - m_Data[index(x - 1, y)]) / (2.f * m_PointSpacing);
    else if (x == 0)
        grad.x = m_Data[index(x + 1, y)] - m_Data[index(x, y)];
    else
        grad.x = m_Data[index(x, y)] - m_Data[index(x - 1, y)];

    if (y > 0 && y < m_Height - 1)
        grad.y = (m_Data[index(x, y + 1)] - m_Data[index(x, y - 1)]) / (2.f * m_PointSpacing);
    else if (y == 0)
        grad.y = m_Data[index(x, y + 1)] - m_Data[index(x, y)];
    else
        grad.y = m_Data[index(x, y)] - m_Data[index(x, y - 1)];

    return grad;
}

inline constexpr ScalarField ScalarField::getGradientNormScalarField() const
{
    ScalarField gradNorm(m_Width, m_Height);

    for (size_t i = 0; i < m_Data.size(); ++i)
    {
        gradNorm.setValue(i, getGradientNorm(i));
    }

    return gradNorm;
}

inline constexpr float ScalarField::getLaplacian(size_t x, size_t y) const
{
    auto fxy = getValue(x, y);
    auto fx1y = x < m_Width - 1 ? getValue(x + 1, y) : fxy;
    auto fx_1y = x > 0 ? getValue(x - 1, y) : fxy;
    auto fxy1 = y < m_Height - 1 ? getValue(x, y + 1) : fxy;
    auto fxy_1 = y > 0 ? getValue(x, y - 1) : fxy;

    return (fx1y + fx_1y + fxy1 + fxy_1 - 4 * fxy) / (m_PointSpacing * m_PointSpacing);
}

inline constexpr ScalarField ScalarField::getLaplacianScalarField() const
{
    ScalarField laplacian(m_Width, m_Height);

    for (size_t i = 0; i < m_Data.size(); i++)
    {
        laplacian.setValue(i, getLaplacian(i));
    }

    return laplacian;
}

inline constexpr float ScalarField::getMin() const
{
    return *std::min_element(m_Data.begin(), m_Data.end());
}

inline constexpr float ScalarField::getMax() const
{
    return *std::max_element(m_Data.begin(), m_Data.end());
}