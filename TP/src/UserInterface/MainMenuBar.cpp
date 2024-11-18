#include "UserInterface/MainMenuBar.h"

#include <imgui.h>

#include <Vroom/Core/Application.h>

MainMenuBar::MainMenuBar()
{
}

MainMenuBar::~MainMenuBar()
{
}

void MainMenuBar::onImgui()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::Separator();

            if (ImGui::MenuItem("Exit", "Alt+F4"))
            {
                vrm::Application::Get().exit();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "Ctrl+Z"))
            {
                notImplemented();
            }
            if (ImGui::MenuItem("Redo", "Ctrl+Shift+Z"))
            {
                notImplemented();
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Cut", "Ctrl+X"))
            {
                notImplemented();
            }
            if (ImGui::MenuItem("Copy", "Ctrl+C"))
            {
                notImplemented();
            }
            if (ImGui::MenuItem("Paste", "Ctrl+V"))
            {
                notImplemented();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Scene", "Ctrl+1"))
            {
                notImplemented();
            }
            if (ImGui::MenuItem("Properties", "Ctrl+2"))
            {
                notImplemented();
            }
            if (ImGui::MenuItem("Console", "Ctrl+3"))
            {
                notImplemented();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("About", "Ctrl+F1"))
            {
                /// @todo Redirect to Github page
                notImplemented();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    
    if (ImGui::BeginPopup("Not implemented"))
    {
        ImGui::Text("This feature is not implemented yet.");
        ImGui::EndPopup();
    }
}
