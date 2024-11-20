#pragma once

#include "UserInterface/ImGuiElement.h"

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
};