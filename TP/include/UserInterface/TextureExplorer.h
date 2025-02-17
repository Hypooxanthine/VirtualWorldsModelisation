#pragma once

#include <map>

#include <Vroom/Render/Abstraction/GLCall.h>
#include <Vroom/Asset/AssetData/TextureData.h>
#include <Vroom/Render/Abstraction/Texture2D.h>

#include "UserInterface/ImGuiElement.h"

class HeightFieldScene;

class TextureExplorer : public ImGuiElement
{
public:
    inline TextureExplorer() = default;
    inline ~TextureExplorer() = default;

    inline void clear()
    {
        m_Textures.clear();
    }

    inline void addOrUpdateTexture(size_t slot, const std::string& name, vrm::ByteTextureData&& texture, float computeTime)
    {
        vrm::Texture2D gpuTexture;
        gpuTexture.loadFromTextureData(texture);
        static GLint swizzle[4] = {
            GL_RED,
            GL_RED,
            GL_RED,
            GL_ONE
        };
        GLCall(glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle));
        m_Textures[slot] = std::make_tuple(name, std::move(texture), std::move(gpuTexture), computeTime);
    }

    inline const vrm::ByteTextureData& getTexture(size_t slot) const
    {
        return std::get<1>(m_Textures.at(slot));
    }

protected:
    void onImgui() override;
    void onInit() override;

private:
    HeightFieldScene* m_Scene = nullptr;
    
    std::map<size_t, std::tuple<std::string, vrm::ByteTextureData, vrm::Texture2D, float>> m_Textures;
    std::string m_SaveName = "texture.png";
};