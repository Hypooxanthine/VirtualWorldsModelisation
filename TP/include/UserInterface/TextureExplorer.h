#pragma once

#include <map>

#include <Vroom/Render/Abstraction/Texture2D.h>

#include "UserInterface/ImGuiElement.h"

class HeightFieldScene;

class TextureExplorer : public ImGuiElement
{
public:
    inline TextureExplorer() = default;
    inline ~TextureExplorer() = default;

    inline void addOrUpdateTexture(size_t slot, const std::string& name, vrm::Texture2D&& texture)
    {
        m_Textures[slot] = std::make_pair(name, std::move(texture));
    }

protected:
    void onImgui() override;
    void onInit() override;

private:
    HeightFieldScene* m_Scene = nullptr;
    
    std::map<size_t, std::pair<std::string, vrm::Texture2D>> m_Textures;
};