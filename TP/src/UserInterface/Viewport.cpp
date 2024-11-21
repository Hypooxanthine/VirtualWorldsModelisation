#include "UserInterface/Viewport.h"

#include <Vroom/Core/Log.h>
#include <Vroom/Render/Abstraction/FrameBuffer.h>

#include "imgui.h"

Viewport::Viewport()
    : frameBuffer(nullptr), m_LastViewportSize(ImVec2(0.f, 0.f))
{
}

Viewport::~Viewport()
{
}

void Viewport::onImgui()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Viewport");
    {    
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        ImGui::BeginChildFrame(ImGui::GetID("ViewportFrame"), ImVec2(0, 0), ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        {
            auto size = ImGui::GetContentRegionAvail();

            if (size.x != m_LastViewportSize.x || size.y != m_LastViewportSize.y)
            {
                m_DidSizeChangeLastFrame = true;
                m_LastViewportSize = size;
            }
            else
            {
                m_DidSizeChangeLastFrame = false;
            }

            if (frameBuffer)
            {
                ImTextureID textureID = (ImTextureID)(intptr_t)frameBuffer->getTexture().getRendererID();
                ImVec2 imageSize = ImVec2(
                    static_cast<float>(frameBuffer->getSpecification().width),
                    static_cast<float>(frameBuffer->getSpecification().height)
                );
                ImGui::Image(textureID, imageSize, ImVec2(0, 1), ImVec2(1, 0));
                
                m_Active = ImGui::IsWindowFocused() && ImGui::IsMouseDragging(ImGuiMouseButton_Left, -.1f);
            }

            ImGui::EndChildFrame();
        }
        
        ImGui::PopStyleVar();
    
        ImGui::End();
    }
    ImGui::PopStyleVar();
}
