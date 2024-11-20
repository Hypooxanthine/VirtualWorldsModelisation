#pragma once

#include <string_view>

class ImGuiElement
{
public:
    ImGuiElement();
    ~ImGuiElement();

    void init();
    void renderImgui();

protected:
    virtual void onInit() {}
    virtual void onImgui() = 0;
    void notImplemented();

};
