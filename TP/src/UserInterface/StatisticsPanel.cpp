#include "UserInterface/StatisticsPanel.h"

#include <imgui.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

StatisticsPanel::StatisticsPanel()
    : frameTime(0.f)
{
}

StatisticsPanel::~StatisticsPanel()
{
}

void StatisticsPanel::updateCameraOutput(const vrm::FirstPersonCamera& camera)
{
    m_CameraOutput = "Camera position: " + glm::to_string(camera.getPosition()) + "\n"
        + "Camera Yaw: " + std::to_string(glm::degrees(camera.getRotation().y)) + "°\n"
        + "Camera Pitch: " + std::to_string(glm::degrees(camera.getRotation().x)) + "°\n";
}

void StatisticsPanel::onImgui()
{
    ImGui::Begin("Statistics");

    ImGui::Text("Frame time: %.5f s", frameTime);
    ImGui::Text("Frame rate: %.0f FPS", 1.f / frameTime);
    ImGui::Text("%s", m_CameraOutput.c_str());

    ImGui::End();
}
