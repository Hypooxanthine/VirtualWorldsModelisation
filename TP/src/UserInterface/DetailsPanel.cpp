#include "UserInterface/DetailsPanel.h"

#include <Vroom/Core/Application.h>
#include <Vroom/Core/GameLayer.h>
#include <Vroom/Scene/Scene.h>

#include "imgui.h"

#include "HeightFieldScene.h"

DetailsPanel::DetailsPanel()
{
}

void DetailsPanel::onImgui()
{
    ImGui::Begin("Details panel");

    if (ImGui::Button("Update mesh"))
    {
        m_Scene->updateMesh();
    }

    ImGui::TextWrapped("Light intensity");
    ImGui::SliderFloat("##Light intensity", &m_Scene->getLightComponent().intensity, 0.f, 1'000'000'000.f, "%.1f", ImGuiSliderFlags_Logarithmic);

    ImGui::End();
}

void DetailsPanel::onInit()
{
    vrm::Scene* scene = &vrm::Application::Get().getGameLayer().getScene();
    if (auto* asHeightFieldScene = dynamic_cast<HeightFieldScene*>(scene))
        m_Scene = asHeightFieldScene;
    else
        VRM_ASSERT_MSG(false, "DetailsPanel: Scene is not of type HeightFieldScene.");
}