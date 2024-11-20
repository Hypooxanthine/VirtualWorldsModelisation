#include "UILayer.h"

#include <Vroom/Core/Application.h>
#include <Vroom/Core/GameLayer.h>
#include <Vroom/Core/Window.h>
#include <Vroom/Scene/Scene.h>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

UILayer::UILayer()
{
    
}

void UILayer::onInit()
{
    auto& app = vrm::Application::Get();
    auto& gameLayer = app.getGameLayer();

    // Engine setup
    app.getGameLayer().getFrameBuffer().setOnScreenRender(false);
    app.getGameLayer().setShouldHandleEvents(false);
    app.getGameLayer().setShouldUpdate(false);
    app.getGameLayer().setShouldRender(true);

    // Frame buffer
    vrm::FrameBuffer::Specification specs = {
        .onScreen = true,
        .width = app.getWindow().getWidth(),
        .height = app.getWindow().getHeight(),
        .useBlending = true,
        .useDepthTest = true,
        .clearColor = {0.1f, 0.1f, 0.1f, 1.0f}
    };
    m_FrameBuffer.create(specs);
    m_FrameBuffer.bind();

    // Imgui setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplGlfw_InitForOpenGL(app.getWindow().getGLFWHandle(), true);
    ImGui_ImplOpenGL3_Init("#version 450");

    m_Font = io.Fonts->AddFontFromFileTTF("Resources/Fonts/Roboto/Roboto-Regular.ttf", 24.0f);
    VRM_ASSERT_MSG(m_Font, "Failed to load font.");

    // Framebuffer management (viewport)
    m_Viewport.setFrameBuffer(&app.getGameLayer().getFrameBuffer());

    // UI setup
    m_MainMenuBar.init();
    m_StatisticsPanel.init();
    m_Viewport.init();
    m_DetailsPanel.init();
}

void UILayer::onEnd()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UILayer::onUpdate(float dt)
{
    auto& app = vrm::Application::Get();

    // Frame time management
    static float accumulator = 0.f;
    static size_t frameCounter = 0;
    accumulator += dt;
    ++frameCounter;
    if (accumulator > 1.f && frameCounter > 0)
    {
        m_StatisticsPanel.frameTime = accumulator / static_cast<float>(frameCounter);
        accumulator = 0.f;
        frameCounter = 0;
    }

    // Handling viewport resize
    if (m_Viewport.didSizeChangeLastFrame())
        onViewportResize();

    // If the viewport is active, game layer has to handle events
    if (m_Viewport.isActive())
    {
        app.getWindow().setCursorVisible(false);
        app.getGameLayer().setShouldHandleEvents(true);
    }
    else
    {
        app.getWindow().setCursorVisible(true);
        app.getGameLayer().setShouldHandleEvents(false);
    }
}

void UILayer::onRender()
{
    m_FrameBuffer.bind();
    m_FrameBuffer.clearColorBuffer();

    onImgui();
}

void UILayer::onEvent(vrm::Event& e)
{
    if (e.type == vrm::Event::Type::Exit)
        vrm::Application::Get().exit();
}

void UILayer::onImgui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::PushFont(m_Font);

    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
    
    m_MainMenuBar.renderImgui();
    m_StatisticsPanel.renderImgui();
    m_Viewport.renderImgui();
    m_DetailsPanel.renderImgui();

    ImGui::PopFont();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UILayer::onViewportResize()
{
    vrm::Event e;
    e.type = vrm::Event::Type::WindowsResized;
    e.newWidth = static_cast<int>(m_Viewport.getLastViewportSize().x);
    e.newHeight = static_cast<int>(m_Viewport.getLastViewportSize().y);
    auto& gameLayer = vrm::Application::Get().getGameLayer();

    // We trick the game layer to handle resize event even if it is not handling events
    // because we want the viewport to be smooth on resize, even when the game isn't playing.
    bool handledEvents = gameLayer.isHandlingEvents();
    gameLayer.setShouldHandleEvents(true);
    gameLayer.submitEvent(e);
    gameLayer.setShouldHandleEvents(handledEvents);
}