#include "ScalarFields/ScalarField.h"

#include <Vroom/Core/Assert.h>

ScalarField::ScalarField()
{

}

ScalarField::ScalarField(size_t width, size_t height)
    : m_Width(width), m_Height(height), m_Data(width * height, 0.5f)
{
}

void ScalarField::setFromTexture(const vrm::Texture2D& texture)
{
    VRM_ASSERT_MSG(texture.getFormat() == vrm::Texture2D::Format::Grayscale, "ScalarField: Texture format must be Grayscale.");

    m_Width = texture.getWidth();
    m_Height = texture.getHeight();
    m_Data.resize(m_Width * m_Height);

    std::vector<unsigned char> pixels = texture.getData();
    
    for (size_t i = 0; i < m_Data.size(); i++)
    {
        m_Data[i] = static_cast<float>(pixels[i] / 255.0f);
    }
}

vrm::Texture2D ScalarField::toTexture(float min, float max) const
{
    std::vector<unsigned char> pixels(m_Width * m_Height);

    for (size_t i = 0; i < m_Data.size(); i++)
    {
        pixels[i] = static_cast<unsigned char>(glm::clamp((m_Data[i] - min) / (max - min) * 255.f, 0.0f, 255.f));
    }

    vrm::Texture2D texture;
    texture.create(static_cast<int>(m_Width), static_cast<int>(m_Height), vrm::Texture2D::Format::Grayscale, pixels.data());

    return texture;
}