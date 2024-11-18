#pragma once

#include "UserInterface/ImGuiElement.h"

class MainMenuBar : public ImGuiElement
{
public:
    MainMenuBar();
    ~MainMenuBar();

protected:
    void onImgui() override;

};
