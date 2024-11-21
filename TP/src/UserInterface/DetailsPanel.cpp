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

    ImGui::TextWrapped("Light transform");
    auto lightPos = m_Scene->getLightTransform().getPosition();
    if (ImGui::SliderFloat3("##Light position", &lightPos.x, -100.f, 100.f, "%.1f"))
        m_Scene->getLightTransform().setPosition(lightPos);
    ImGui::TextWrapped("Light intensity");
    ImGui::SliderFloat("##Light intensity", &m_Scene->getLightComponent().intensity, 0.f, 1'000'000'000.f, "%.1f", ImGuiSliderFlags_Logarithmic);
    ImGui::TextWrapped("Light color");
    ImGui::SliderFloat3("##Light color", &m_Scene->getLightComponent().color.x, 0.f, 1.f, "%.01f");

    auto scale = m_Scene->getMeshTransform().getScale();
    ImGui::TextWrapped("Scale");
    if (ImGui::SliderFloat3("##Scale", &scale.x, 0.01f, 1.f, "%.3f"))
        m_Scene->getMeshTransform().setScale(scale);

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