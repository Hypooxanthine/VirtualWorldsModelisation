#pragma once

#include <Vroom/Core/Layer.h>

#include "imgui.h"

#include <Vroom/Render/Abstraction/FrameBuffer.h>
#include <Vroom/Render/Camera/FirstPersonCamera.h>
#include <Vroom/Event/Trigger/TriggerManager.h>
#include <Vroom/Event/CustomEvent/CustomEventManager.h>
#include <glm/gtc/constants.hpp>

#include "UserInterface/MainMenuBar.h"
#include "UserInterface/StatisticsPanel.h"
#include "UserInterface/Viewport.h"
#include "UserInterface/DetailsPanel.h"

class UILayer final : public vrm::Layer
{
public:
    UILayer();

    void onInit();
    virtual void onEnd();
    virtual void onUpdate(float dt);
    virtual void onRender();
    virtual void onEvent(vrm::Event& e);

private:
    void onImgui();

    void onViewportResize();

private:
    vrm::FrameBuffer m_FrameBuffer;
    ImFont* m_Font;

    // Events
    vrm::TriggerManager m_Triggers;
    vrm::CustomEventManager m_CustomEvents;

    // Camera params
    vrm::FirstPersonCamera m_Camera;
    float forwardValue = 0.f, rightValue = 0.f, upValue = 0.f;
	float turnRightValue = 0.f, lookUpValue = 0.f;
	float myCameraSpeed = 10.f, myCameraAngularSpeed = .08f * glm::two_pi<float>() / 360.f;

    // UI
    MainMenuBar m_MainMenuBar;
    StatisticsPanel m_StatisticsPanel;
    Viewport m_Viewport;
    DetailsPanel m_DetailsPanel;
};