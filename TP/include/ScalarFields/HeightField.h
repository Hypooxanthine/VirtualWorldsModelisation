#pragma once

#include "ScalarFields/ScalarField.h"

class HeightField : public ScalarField
{
public:
    inline constexpr HeightField() {}
    inline constexpr HeightField(size_t width, size_t height) : ScalarField(width, height) {}

    inline float getSlope(size_t x, size_t y) const
    {
        glm::vec2 grad = getGradient(x, y);
        return glm::length(grad);
    }

    float getAverageSlope(size_t x, size_t y) const;

    size_t getWidth() const = delete;
    inline constexpr size_t getSizeX() const { return ScalarField::getWidth(); }

    size_t getHeight() const = delete;
    inline constexpr size_t getSizeY() const { return ScalarField::getHeight(); }

    float getValue(size_t i) const = delete;
    float getValue(size_t x, size_t y) const = delete;
    inline constexpr float getHeight(size_t i) const { return ScalarField::getValue(i); }
    inline constexpr float getHeight(size_t x, size_t y) const { return ScalarField::getValue(x, y); }

    void setValue(size_t i) = delete;
    void setValue(size_t i, float value) = delete;
    inline constexpr void setHeight(size_t i, float height) { ScalarField::setValue(i, height); }
    inline constexpr void setHeight(size_t x, size_t y, float height) { ScalarField::setValue(x, y, height); }
};