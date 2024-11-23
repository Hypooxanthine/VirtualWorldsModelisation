#include "ScalarFields/ScalarField.h"

#include <Vroom/Core/Assert.h>
#include <Vroom/Render/Abstraction/GLCall.h>

void ScalarField::setFromTexture(const vrm::Texture2D& texture)
{
    if (texture.getFormat() != vrm::Texture2D::Format::Grayscale)
    {
        VRM_LOG_WARN("ScalarField: Format isn't Grayscale, only red channel will be used.");
    }

    m_Width = texture.getWidth();
    m_Height = texture.getHeight();
    m_Data.resize(m_Width * m_Height);

    std::vector<unsigned char> pixels = texture.getData();

    VRM_LOG_TRACE("ScalarField: Texture size: {}x{}, bpp: {}", m_Width, m_Height, texture.getBPP());

    for (size_t i = 0; i < m_Width * m_Height; i++)
    {
        m_Data[i] = static_cast<float>(pixels[i * texture.getBPP()]);
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
    static GLint swizzle[4] = {
        GL_RED,
        GL_RED,
        GL_RED,
        GL_ONE
    };
    GLCall(glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle));

    return texture;
}
