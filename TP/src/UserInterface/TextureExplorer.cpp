#include "UserInterface/TextureExplorer.h"

#include "imgui.h"

void TextureExplorer::onInit()
{
}

void TextureExplorer::onImgui()
{
    ImGui::Begin("Texture explorer");
    ImGui::BeginTabBar("Textures");

    for (auto& [slot, pair] : m_Textures)
    {
        auto& [name, texture] = pair;
        if (ImGui::BeginTabItem(name.c_str()))
        {
            ImGui::Image(
                (ImTextureID)texture.getRendererID(),
                ImVec2(texture.getWidth(), texture.getHeight()),
                ImVec2(0, 1), ImVec2(1, 0)
            );

            ImGui::EndTabItem();
        }
    }
    
    ImGui::EndTabBar();
    ImGui::End();
}