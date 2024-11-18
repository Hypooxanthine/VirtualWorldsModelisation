#pragma once

#include <string_view>

class ImGuiElement
{
public:
    ImGuiElement();
    ~ImGuiElement();

    void renderImgui();

protected:
    virtual void onImgui() = 0;
    void notImplemented();

};
