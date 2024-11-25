#include "UserInterface/DetailsPanel.h"

#include <Vroom/Core/Application.h>
#include <Vroom/Core/GameLayer.h>
#include <Vroom/Scene/Scene.h>

#include "imgui.h"

#include "HeightFieldScene.h"

DetailsPanel::DetailsPanel()
{
}

void DetailsPanel::onInit()
{
    vrm::Scene* scene = &vrm::Application::Get().getGameLayer().getScene();
    if (auto* asHeightFieldScene = dynamic_cast<HeightFieldScene*>(scene))
        m_Scene = asHeightFieldScene;
    else
        VRM_ASSERT_MSG(false, "DetailsPanel: Scene is not of type HeightFieldScene.");

    m_TextureExplorer.init();

    m_Scene->updateHeightField(m_HeightFieldSpecs);
}

void DetailsPanel::onImgui()
{
    if (ImGui::Begin("Details panel"))
    {
        if (ImGui::Button("Update mesh"))
        {
            m_Scene->updateMesh();
        }

        if (ImGui::Button("Update textures"))
        {
            m_TextureExplorer.addOrUpdateTexture(
                0,
                "Height",
                m_Scene->getHeightField().toTexture()
            );
            m_TextureExplorer.addOrUpdateTexture(
                1,
                "Slope",
                m_Scene->getHeightField().getSlopeScalarField().toTexture()
            );
            m_TextureExplorer.addOrUpdateTexture(
                2,
                "Average slope",
                m_Scene->getHeightField().getAverageSlopeScalarField().toTexture()
            );
            m_TextureExplorer.addOrUpdateTexture(
                3,
                "Laplacian",
                m_Scene->getHeightField().getLaplacianScalarField().toTexture()
            );
        }

        ImGui::TextWrapped("Light position");
        auto lightPos = m_Scene->getLightTransform().getPosition();
        if (ImGui::SliderFloat3("##Light position", &lightPos.x, -100.f, 100.f, "%.1f"))
            m_Scene->getLightTransform().setPosition(lightPos);
        ImGui::TextWrapped("Light intensity");
        ImGui::SliderFloat("##Light intensity", &m_Scene->getLightComponent().intensity, 0.f, 1'000'000'000.f, "%.1f", ImGuiSliderFlags_Logarithmic);
        ImGui::TextWrapped("Light color");
        ImGui::ColorPicker3("##Light color", &m_Scene->getLightComponent().color.x);

        ImGui::TextWrapped("Distance between points");
        if (ImGui::SliderFloat("##Point spacing", &m_HeightFieldSpecs.pointSpacing, 0.01f, 10.f, "%.3f"))
            m_Scene->updateHeightField(m_HeightFieldSpecs);

        ImGui::TextWrapped("Min/max height");
        if (ImGui::SliderFloat2("##Min/max height", m_HeightFieldSpecs.zMinMax.data(), -100.f, 100.f))
            m_Scene->updateHeightField(m_HeightFieldSpecs);

        auto scale = m_Scene->getMeshTransform().getScale();
        ImGui::TextWrapped("Scale");
        if (ImGui::SliderFloat3("##Scale", &scale.x, 0.01f, 1.f, "%.3f"))
            m_Scene->getMeshTransform().setScale(scale);

        ImGui::End();
    }

    m_TextureExplorer.renderImgui();
}
