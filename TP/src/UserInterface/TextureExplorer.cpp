#include "UserInterface/TextureExplorer.h"

#include "imgui.h"

void TextureExplorer::onInit()
{
}

void TextureExplorer::onImgui()
{
    ImGui::Begin("Texture explorer");
    ImGui::BeginTabBar("Textures");

    for (auto& [slot, tuple] : m_Textures)
    {
        auto& [name, texture, gpuTexture] = tuple;
        if (ImGui::BeginTabItem(name.c_str()))
        {
            float textureSizeRatio = texture.getWidth() / texture.getHeight();
            ImVec2 availableSize = ImGui::GetContentRegionAvail();
            float availableSizeRatio = availableSize.x / availableSize.y;
            ImVec2 desiredSize;

            if (textureSizeRatio > availableSizeRatio)
                desiredSize = ImVec2(availableSize.x, availableSize.x / textureSizeRatio);
            else
                desiredSize = ImVec2(availableSize.y * textureSizeRatio, availableSize.y);

            ImGui::Image(
                (ImTextureID)gpuTexture.getRendererID(),
                desiredSize,
                ImVec2(0, 1), ImVec2(1, 0)
            );

            ImGui::EndTabItem();
        }
    }
    
    ImGui::EndTabBar();
    ImGui::End();
}