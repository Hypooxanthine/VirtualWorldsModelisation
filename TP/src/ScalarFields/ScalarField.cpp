#include "ScalarFields/ScalarField.h"

#include <Vroom/Core/Assert.h>
#include <Vroom/Render/Abstraction/GLCall.h>

void ScalarField::setFromTexture(const vrm::ByteTextureData& texture, const FromTextureSpecs& specs)
{
    if (texture.getFormat() != vrm::TextureFormat::Grayscale)
    {
        VRM_LOG_WARN("ScalarField: Format isn't Grayscale, only red channel will be used.");
    }

    m_Width = texture.getWidth();
    m_Height = texture.getHeight();
    m_PointSpacing = specs.pointSpacing;
    m_Data.resize(m_Width * m_Height);

    const unsigned char* pixels = texture.getData();

    VRM_LOG_TRACE("ScalarField: Texture size: {}x{}, bpp: {}", m_Width, m_Height, texture.getBpp());

    for (size_t i = 0; i < m_Width * m_Height; i++)
    {
        const float normalized = static_cast<float>(pixels[i * texture.getBpp()] / 255.f);
        m_Data[i] = specs.zmin + normalized * (specs.zmax - specs.zmin);
    }
}

vrm::ByteTextureData ScalarField::toTexture(float min, float max) const
{
    VRM_LOG_INFO("To texture, min: {}, max: {}", min, max);
    std::vector<unsigned char> pixels(m_Width * m_Height);

    for (size_t i = 0; i < m_Data.size(); i++)
    {
        pixels[i] = static_cast<unsigned char>(glm::clamp((m_Data[i] - min) / (max - min) * 255.f, 0.0f, 255.f));
    }

    vrm::ByteTextureData texture;
    texture.create(static_cast<int>(m_Width), static_cast<int>(m_Height), 1, pixels.data());
    static GLint swizzle[4] = {
        GL_RED,
        GL_RED,
        GL_RED,
        GL_ONE
    };
    GLCall(glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle));

    return texture;
}
