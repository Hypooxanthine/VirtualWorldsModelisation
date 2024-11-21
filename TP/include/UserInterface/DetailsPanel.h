#pragma once

#include "UserInterface/ImGuiElement.h"
#include "UserInterface/TextureExplorer.h"

class HeightFieldScene;

class DetailsPanel : public ImGuiElement
{
public:
    DetailsPanel();

protected:
    void onImgui() override;
    void onInit() override;

private:
    HeightFieldScene* m_Scene = nullptr;
    TextureExplorer m_TextureExplorer;
};