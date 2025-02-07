#pragma once

#include "UserInterface/ImGuiElement.h"
#include "UserInterface/TextureExplorer.h"

#include "ScalarFields/ScalarField.h"
#include "ScalarFields/HeightField.h"

class HeightFieldScene;

class DetailsPanel : public ImGuiElement
{
public:
    DetailsPanel();

protected:
    void onImgui() override;
    void onInit() override;

private:
    void showTexturesLoader();
    void updateTextures();
    void updateHeightField(bool adjustMinMax);

private:
    HeightFieldScene* m_Scene = nullptr;
    HeightField m_HeightField;
    ScalarField m_SlopeField, m_AverageSlopeField, m_LaplacianField, m_DrainageAreaField, m_StreamPowerField, m_WetnessIndexField;
    ScalarField m_normalizedHeightField, m_normalizedSlopeField, m_normalizedAverageSlopeField, m_normalizedLaplacianField, m_normalizedDrainageAreaField, m_normalizedStreamPowerField, m_normalizedWetnessIndexField;
    TextureExplorer m_TextureExplorer;

    ScalarField::FromTextureSpecs m_HeightFieldSpecs = ScalarField::FromTextureSpecs{
        .pointSpacing = 1.f,
        .zMinMax = { 0.f, 8.f }
    };

    bool m_HighlightSingleEnabled = false;
    glm::ivec2 m_HighlightCoords = { 0, 0 }, m_HighlightStartPathCoords = { 0, 0 }, m_HighlightEndPathCoords = { 0, 0 };

    float m_LastDijkstraTime = -1.f;
    float m_DistanceImportance = 1.f, m_HeightImportance = 0.f, m_SlopeImportance = 0.f, m_AverageSlopeImportance = 0.f, m_LaplacianImportance = 0.f, m_DrainageAreaImportance = 0.f, m_StreamPowerImportance = 0.f, m_WetnessIndexImportance = 0.f;
};