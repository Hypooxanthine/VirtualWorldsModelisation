#pragma once

#include <Vroom/Core/Layer.h>

#include "imgui.h"

#include <Vroom/Render/Abstraction/FrameBuffer.h>

#include "UserInterface/MainMenuBar.h"
#include "UserInterface/StatisticsPanel.h"
#include "UserInterface/Viewport.h"

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

    // UI
    MainMenuBar m_MainMenuBar;
    StatisticsPanel m_StatisticsPanel;
    Viewport m_Viewport;
};