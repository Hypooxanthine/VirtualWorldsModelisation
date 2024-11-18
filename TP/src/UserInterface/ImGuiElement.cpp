#include "UserInterface/ImGuiElement.h"

#include <imgui.h>

#include <Vroom/Core/Log.h>

ImGuiElement::ImGuiElement()
{
}

ImGuiElement::~ImGuiElement()
{
}

void ImGuiElement::renderImgui()
{
    onImgui();
}

void ImGuiElement::notImplemented()
{
    VRM_LOG_WARN("Not implemented");
}
