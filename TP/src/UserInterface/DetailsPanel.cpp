#include "UserInterface/DetailsPanel.h"

#include <filesystem>

#include <Vroom/Core/Application.h>
#include <Vroom/Core/GameLayer.h>
#include <Vroom/Scene/Scene.h>

#include "imgui.h"

#include "HeightFieldScene.h"

#include "ScopeProfiler.h"

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
}

void DetailsPanel::onImgui()
{
    if (ImGui::Begin("Details panel"))
    {
        showTexturesLoader();

        ImGui::Separator();

        if (!m_Scene->getHeightMap().isCreated())
            ImGui::BeginDisabled();

        ImGui::TextWrapped("Distance between points");
        ImGui::SliderFloat("##Point spacing", &m_HeightFieldSpecs.pointSpacing, 0.01f, 10.f, "%.3f");

        ImGui::TextWrapped("Min/max height");
        ImGui::SliderFloat2("##Min/max height", m_HeightFieldSpecs.zMinMax.data(), -100.f, 100.f);

        if (ImGui::Button("Update height field"))
            updateHeightField(false);

        if (ImGui::Button("Update textures"))
            updateTextures();

        if (!m_Scene->getHeightMap().isCreated())
            ImGui::EndDisabled();

        ImGui::Separator();

        ImGui::TextWrapped("Light position");
        auto lightPos = m_Scene->getLightTransform().getPosition();
        if (ImGui::SliderFloat3("##Light position", &lightPos.x, -100.f, 100.f, "%.1f"))
            m_Scene->getLightTransform().setPosition(lightPos);

        ImGui::TextWrapped("Light intensity");
        ImGui::SliderFloat("##Light intensity", &m_Scene->getLightComponent().intensity, 0.f, 1'000'000'000.f, "%.1f", ImGuiSliderFlags_Logarithmic);

        ImGui::TextWrapped("Light color");
        ImGui::ColorPicker3("##Light color", &m_Scene->getLightComponent().color.x);

        auto scale = m_Scene->getMeshTransform().getScale();
        ImGui::TextWrapped("Scale");
        if (ImGui::SliderFloat3("##Scale", &scale.x, 0.01f, 10.f, "%.3f"))
        {
            m_Scene->getMeshTransform().setScale(scale);
            if (m_HighlightEnabled)
                m_Scene->highlightPoint(m_HighlightCoords.x, m_HighlightCoords.y, m_HighlightRadius);
        }

        ImGui::Separator();

        if (m_Scene->getHeightField().isValidIndex(0)
            && ImGui::Checkbox("Enable vertex highlighting test", &m_HighlightEnabled))
        {
            m_Scene->enableHighlightSingle(m_HighlightEnabled);
            if (m_HighlightEnabled)
                m_Scene->highlightPoint(m_HighlightCoords.x, m_HighlightCoords.y, m_HighlightRadius);
        }

        if (m_HighlightEnabled)
        {
            ImGui::TextWrapped("Highlight radius");
            if (ImGui::SliderFloat("##Highlight radius", &m_HighlightRadius, 0.1f, 10.f, "%.3f"))
                m_Scene->highlightPoint(m_HighlightCoords.x, m_HighlightCoords.y, m_HighlightRadius);
            
            ImGui::TextWrapped("Highlight vertex");
            if (ImGui::SliderInt("##Highlight x", &m_HighlightCoords.x, 0, m_Scene->getHeightField().getSizeX() - 1) ||
                ImGui::SliderInt("##Highlight y", &m_HighlightCoords.y, 0, m_Scene->getHeightField().getSizeY() - 1))
            {
                m_Scene->highlightPoint(m_HighlightCoords.x, m_HighlightCoords.y, m_HighlightRadius);
            }
        }

        ImGui::End();
    }

    m_TextureExplorer.renderImgui();
}

void DetailsPanel::showTexturesLoader()
{
    static std::string selectedTexture = "Select texture";
    if (ImGui::BeginCombo("##Texture", selectedTexture.c_str()))
    {
        for (const auto& entry : std::filesystem::directory_iterator("Resources/Textures"))
        {
            if (entry.is_regular_file())
            {
                std::string filename = entry.path().filename().string();
                if (ImGui::Selectable(filename.c_str()))
                {
                    auto path = entry.path().string();
                    selectedTexture = filename;

                    m_Scene->reloadTexture(path);
                    updateHeightField(true);
                }
            }
        }

        ImGui::EndCombo();
    }
}

void DetailsPanel::updateTextures()
{
    m_TextureExplorer.clear();

    float t;
    vrm::ByteTextureData map;
    {
        PROFILE_SCOPE_VARIABLE(t);
        map = vrm::ByteTextureData(m_Scene->getHeightMap());
    }
    m_TextureExplorer.addOrUpdateTexture(
        0,
        "Height",
        std::move(map),
        t
    );

    {
        PROFILE_SCOPE_VARIABLE(t);
        map = m_Scene->getHeightField().getSlopeScalarField().toTexture();
    }
    m_TextureExplorer.addOrUpdateTexture(
        1,
        "Slope",
        std::move(map),
        t
    );

    {
        PROFILE_SCOPE_VARIABLE(t);
        map = m_Scene->getHeightField().getAverageSlopeScalarField().toTexture();
    }
    m_TextureExplorer.addOrUpdateTexture(
        2,
        "Average slope",
        std::move(map),
        t
    );

    {
        PROFILE_SCOPE_VARIABLE(t);
        map = m_Scene->getHeightField().getLaplacianScalarField().toTexture();
    }
    m_TextureExplorer.addOrUpdateTexture(
        3,
        "Laplacian",
        std::move(map),
        t
    );

    {
        PROFILE_SCOPE_VARIABLE(t);
        map = m_Scene->getHeightField().getDrainageAreaScalarField(6.f).toTexture();
    }
    m_TextureExplorer.addOrUpdateTexture(
        4,
        "Drainage area",
        std::move(map),
        t
    );

    {
        PROFILE_SCOPE_VARIABLE(t);
        map = m_Scene->getHeightField().getStreamPowerScalarField().toTexture();
    }
    m_TextureExplorer.addOrUpdateTexture(
        5,
        "Stream power",
        std::move(map),
        t
    );

    {
        PROFILE_SCOPE_VARIABLE(t);
        map = m_Scene->getHeightField().getWetnessIndexScalarField().toTexture();
    }
    m_TextureExplorer.addOrUpdateTexture(
        6,
        "Wetness index",
        std::move(map),
        t
    );
}

void DetailsPanel::updateHeightField(bool adjustMinMax)
{
    if (adjustMinMax)
    {
        const auto& texture = m_Scene->getHeightMap();
        int maxSize = std::max(texture.getWidth(), texture.getHeight());
        m_HeightFieldSpecs.pointSpacing = 50.f / maxSize;
    }
    m_Scene->updateHeightField(m_HeightFieldSpecs);
}