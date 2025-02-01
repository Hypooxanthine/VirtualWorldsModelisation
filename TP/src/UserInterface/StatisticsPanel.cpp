#include "UserInterface/StatisticsPanel.h"

#include <imgui.h>
#include <GLFW/glfw3.h>

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

    if (ImGui::Checkbox("Enable V-Sync", &m_VSync))
    {
        if (m_VSync)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);
    }

    ImGui::End();
}
