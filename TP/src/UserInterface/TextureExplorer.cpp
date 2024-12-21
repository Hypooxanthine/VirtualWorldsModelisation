#include "UserInterface/TextureExplorer.h"

#include "imgui.h"

#include "HeightFieldScene.h"

void TextureExplorer::onInit()
{
    m_SaveName.reserve(256);
}

void TextureExplorer::onImgui()
{
    ImGui::Begin("Texture explorer");
    ImGui::BeginTabBar("Textures");

    for (auto& [slot, tuple] : m_Textures)
    {
        auto& [name, texture, gpuTexture, computeTime] = tuple;
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

            ImGui::Text("Compute time: %.6f s", computeTime);

            if (ImGui::Button("Save as"))
            {
                if (texture.saveToFile(m_SaveName))
                    VRM_LOG_INFO("Texture saved as {}", m_SaveName);
                else
                    VRM_LOG_ERROR("Failed to save texture as {}", m_SaveName);
            }
            ImGui::SameLine();
            ImGui::InputText("##SaveName", m_SaveName.data(), m_SaveName.capacity());

            ImGui::EndTabItem();
        }
    }
    
    ImGui::EndTabBar();
    ImGui::End();
}