#include "UserInterface/DetailsPanel.h"

#include <filesystem>

#include <Vroom/Core/Application.h>
#include <Vroom/Core/GameLayer.h>
#include <Vroom/Scene/Scene.h>

#include "imgui.h"

#include "HeightFieldScene.h"

#include "ScopeProfiler.h"

#include "Graphs/FieldGraph.h"

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
            if (m_HighlightSingleEnabled)
            {
                m_Scene->highlightPoint(m_HighlightCoords.x, m_HighlightCoords.y);
            }
        }

        if (m_Scene->getHeightField().isValidIndex(0))
        {
            ImGui::Separator();

            static int min[2] = { 0, 0 };
            static int max[2];
            max[0] = static_cast<int>(m_Scene->getHeightField().getSizeX());
            max[1] = static_cast<int>(m_Scene->getHeightField().getSizeY());

            ImGui::TextWrapped("Highlight path start/end");
            if (ImGui::SliderScalarN("##Path start", ImGuiDataType_S32, &m_HighlightStartPathCoords.x, 2, min, max))
            {
                m_Scene->highlightStartPath(m_HighlightStartPathCoords.x, m_HighlightStartPathCoords.y);
            }

            if (ImGui::SliderScalarN("##Path end", ImGuiDataType_S32, &m_HighlightEndPathCoords.x, 2, min, max))
            {
                m_Scene->highlightEndPath(m_HighlightEndPathCoords.x, m_HighlightEndPathCoords.y);
            }
            ImGui::TextWrapped("Highlight radius");
            float radius = m_Scene->getHighlightRadius();
            if (ImGui::SliderFloat("##Highlight radius", &radius, 0.1f, 10.f, "%.3f"))
                m_Scene->setHighlightRadius(radius);

            static float minImportance = -10.f;
            static float maxImportance = 10.f;

            ImGui::TextWrapped("Distance importance");
            ImGui::SliderFloat("##Distance importance", &m_DistanceImportance, minImportance, maxImportance, "%.3f");

            ImGui::TextWrapped("Height importance");
            ImGui::SliderFloat("##Height importance", &m_HeightImportance, minImportance, maxImportance, "%.3f");

            ImGui::TextWrapped("Slope importance");
            ImGui::SliderFloat("##Slope importance", &m_SlopeImportance, minImportance, maxImportance, "%.3f");

            ImGui::TextWrapped("Average slope importance");
            ImGui::SliderFloat("##Average slope importance", &m_AverageSlopeImportance, minImportance, maxImportance, "%.3f");

            ImGui::TextWrapped("Laplacian importance");
            ImGui::SliderFloat("##Laplacian importance", &m_LaplacianImportance, minImportance, maxImportance, "%.3f");

            ImGui::TextWrapped("Drainage area importance");
            ImGui::SliderFloat("##Drainage area importance", &m_DrainageAreaImportance, minImportance, maxImportance, "%.3f");

            ImGui::TextWrapped("Stream power importance");
            ImGui::SliderFloat("##Stream power importance", &m_StreamPowerImportance, minImportance, maxImportance, "%.3f");

            ImGui::TextWrapped("Wetness index importance");
            ImGui::SliderFloat("##Wetness index importance", &m_WetnessIndexImportance, minImportance, maxImportance, "%.3f");

            if (ImGui::Button("Dijkstra"))
            {
                PROFILE_SCOPE_VARIABLE(m_LastDijkstraTime);
                
                auto weightFunction = [this](int from, int to) -> float
                {
                    float w = 0.f;

                    glm::vec3 fromPos = m_HeightField.getLocalPosition(from);
                    glm::vec3 toPos = m_HeightField.getLocalPosition(to);

                    float yDiff = toPos.y - fromPos.y;
                    float yPlaneDist = glm::length(glm::vec2(toPos.x, toPos.z) - glm::vec2(fromPos.x, fromPos.z));

                    w += m_DistanceImportance * glm::length(toPos - fromPos);
                    w += m_HeightImportance * m_normalizedHeightField.getValue(to);
                    w += m_SlopeImportance * yDiff / yPlaneDist;
                    w += m_AverageSlopeImportance * m_normalizedAverageSlopeField.getValue(to);
                    w += m_LaplacianImportance * m_normalizedLaplacianField.getValue(to);
                    w += m_DrainageAreaImportance * m_normalizedDrainageAreaField.getValue(to);
                    w += m_StreamPowerImportance * m_normalizedStreamPowerField.getValue(to);
                    w += m_WetnessIndexImportance * m_normalizedWetnessIndexField.getValue(to);


                    return w;
                };

                FieldGraph g(m_Scene->getHeightField().getSizeX(), m_Scene->getHeightField().getSizeY(), weightFunction);

                auto path = g.shortestPath(m_HighlightStartPathCoords.x, m_HighlightStartPathCoords.y, m_HighlightEndPathCoords.x, m_HighlightEndPathCoords.y);
                m_Scene->startPathAnimation(path);
            }

            if (m_LastDijkstraTime > 0.f)
            {
                ImGui::SameLine();
                ImGui::TextWrapped("%.3f ms", m_LastDijkstraTime);
            }

            ImGui::TextWrapped("Path animation time");
            float animTime = m_Scene->getPathAnimationTime();
            if (ImGui::DragFloat("##Path animation time", &animTime, 0.1f, 0.1f, 10.f, "%.3f"))
                m_Scene->setPathAnimationTime(animTime);

            if (ImGui::Checkbox("Enable single vertex highlighting", &m_HighlightSingleEnabled))
            {
                m_Scene->enableHighlightSingle(m_HighlightSingleEnabled);
                if (m_HighlightSingleEnabled)
                    m_Scene->highlightPoint(m_HighlightCoords.x, m_HighlightCoords.y);
            }

            if (m_HighlightSingleEnabled)
            {                
                ImGui::TextWrapped("Highlight vertex");

                if (ImGui::SliderScalarN("##Highlight coords", ImGuiDataType_S32, &m_HighlightCoords.x, 2, min, max))
                {
                    m_Scene->highlightPoint(m_HighlightCoords.x, m_HighlightCoords.y);
                }
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
        m_HeightField = m_Scene->getHeightField();
    }
    m_TextureExplorer.addOrUpdateTexture(
        0,
        "Height",
        std::move(map),
        t
    );

    {
        PROFILE_SCOPE_VARIABLE(t);
        m_SlopeField = m_Scene->getHeightField().getSlopeScalarField();
        map = m_SlopeField.toTexture();
    }
    m_TextureExplorer.addOrUpdateTexture(
        1,
        "Slope",
        std::move(map),
        t
    );

    {
        PROFILE_SCOPE_VARIABLE(t);
        m_AverageSlopeField = m_Scene->getHeightField().getAverageSlopeScalarField();
        map = m_AverageSlopeField.toTexture();
    }
    m_TextureExplorer.addOrUpdateTexture(
        2,
        "Average slope",
        std::move(map),
        t
    );

    {
        PROFILE_SCOPE_VARIABLE(t);
        m_LaplacianField = m_Scene->getHeightField().getLaplacianScalarField();
        map = m_LaplacianField.toTexture();
    }
    m_TextureExplorer.addOrUpdateTexture(
        3,
        "Laplacian",
        std::move(map),
        t
    );

    {
        PROFILE_SCOPE_VARIABLE(t);
        m_DrainageAreaField = m_Scene->getHeightField().getDrainageAreaScalarField(6.f);
        map = m_DrainageAreaField.toTexture();
    }
    m_TextureExplorer.addOrUpdateTexture(
        4,
        "Drainage area",
        std::move(map),
        t
    );

    {
        PROFILE_SCOPE_VARIABLE(t);
        m_StreamPowerField = m_Scene->getHeightField().getStreamPowerScalarField();
        map = m_StreamPowerField.toTexture();
    }
    m_TextureExplorer.addOrUpdateTexture(
        5,
        "Stream power",
        std::move(map),
        t
    );

    {
        PROFILE_SCOPE_VARIABLE(t);
        m_WetnessIndexField = m_Scene->getHeightField().getWetnessIndexScalarField();
        map = m_WetnessIndexField.toTexture();
    }
    m_TextureExplorer.addOrUpdateTexture(
        6,
        "Wetness index",
        std::move(map),
        t
    );

    m_normalizedHeightField = m_HeightField.normalized();
    m_normalizedSlopeField = m_SlopeField.normalized();
    m_normalizedAverageSlopeField = m_AverageSlopeField.normalized();
    m_normalizedLaplacianField = m_LaplacianField.normalized();
    m_normalizedDrainageAreaField = m_DrainageAreaField.normalized();
    m_normalizedStreamPowerField = m_StreamPowerField.normalized();
    m_normalizedWetnessIndexField = m_WetnessIndexField.normalized();
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