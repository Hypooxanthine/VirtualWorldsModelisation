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