#pragma once

#include "UserInterface/ImGuiElement.h"

class StatisticsPanel : public ImGuiElement
{
public:
    StatisticsPanel();
    ~StatisticsPanel();

protected:
    void onImgui() override;

public: // Public ImGui related variables
    float frameTime;

};
