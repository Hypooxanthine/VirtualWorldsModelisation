#pragma once

#include "UserInterface/ImGuiElement.h"
#include "UserInterface/TextureExplorer.h"

#include "ScalarFields/ScalarField.h"

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
    TextureExplorer m_TextureExplorer;

    ScalarField::FromTextureSpecs m_HeightFieldSpecs = ScalarField::FromTextureSpecs{
        .pointSpacing = 1.f,
        .zMinMax = { 0.f, 8.f }
    };
};