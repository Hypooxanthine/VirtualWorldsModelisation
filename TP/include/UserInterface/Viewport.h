#pragma once

#include "UserInterface/ImGuiElement.h"
#include "imgui.h"

namespace vrm
{
class FrameBuffer;
}

class Viewport : public ImGuiElement
{
public:
    Viewport();
    ~Viewport();

    inline const ImVec2& getLastViewportSize() const { return m_LastViewportSize; }
    inline bool didSizeChangeLastFrame() const { return m_DidSizeChangeLastFrame; }

    inline vrm::FrameBuffer* getFrameBuffer() const { return frameBuffer; }
    inline bool isActive() const { return m_Active; }

    inline void setFrameBuffer(vrm::FrameBuffer* fb) { frameBuffer = fb; }

protected:
    void onImgui() override;

private: // ImGui related variables
    vrm::FrameBuffer* frameBuffer;
    
    bool m_Active = false;

private:
    bool m_DidSizeChangeLastFrame = false;
    ImVec2 m_LastViewportSize;

};
