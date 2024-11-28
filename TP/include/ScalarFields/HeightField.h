#pragma once

#include "ScalarFields/ScalarField.h"

class HeightField : public ScalarField
{
public:
    inline constexpr HeightField() {}
    inline constexpr HeightField(size_t width, size_t height) : ScalarField(width, height) {}

    vrm::MeshData toMeshData() const;

    inline float getSlope(size_t x, size_t y) const { return ScalarField::getGradientNorm(x, y); }
    inline float getSlope(size_t i) const { return ScalarField::getGradientNorm(i); }
    inline ScalarField getSlopeScalarField() const { return ScalarField::getGradientNormScalarField(); }

    float getAverageSlope(size_t x, size_t y) const;
    inline float getAverageSlope(size_t i) const { return getAverageSlope(xCoord(i), yCoord(i)); }
    inline ScalarField getAverageSlopeScalarField() const;

    ScalarField getDrainageAreaScalarField(float power = 1.f) const;

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

inline ScalarField HeightField::getAverageSlopeScalarField() const
{
    ScalarField averageSlopeField(getSizeX(), getSizeY());

    for (size_t y = 0; y < getSizeY(); ++y)
    {
        for (size_t x = 0; x < getSizeX(); ++x)
        {
            averageSlopeField.setValue(x, y, getAverageSlope(x, y));
        }
    }

    return averageSlopeField;
}