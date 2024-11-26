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
            ImGui::Image(
                (ImTextureID)gpuTexture.getRendererID(),
                ImVec2(
                    static_cast<float>(texture.getWidth()),
                    static_cast<float>(texture.getHeight())
                ),
                ImVec2(0, 1), ImVec2(1, 0)
            );

            ImGui::EndTabItem();
        }
    }
    
    ImGui::EndTabBar();
    ImGui::End();
}