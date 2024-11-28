#pragma once

#include <string>
#include <Vroom/Render/Camera/FirstPersonCamera.h>

#include "UserInterface/ImGuiElement.h"

class StatisticsPanel : public ImGuiElement
{
public:
    StatisticsPanel();
    ~StatisticsPanel();

    void updateCameraOutput(const vrm::FirstPersonCamera& camera);

protected:
    void onImgui() override;

public: // Public ImGui related variables
    float frameTime;

private:
    std::string m_CameraOutput;
};
