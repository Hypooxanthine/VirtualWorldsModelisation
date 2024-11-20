#include "HeightFieldScene.h"

#include <Vroom/Core/Application.h>
#include <Vroom/Core/GameLayer.h>
#include <Vroom/Scene/Components/MeshComponent.h>
#include <Vroom/Scene/Components/TransformComponent.h>

HeightFieldScene::HeightFieldScene()
    : m_HeightField(), m_Camera(0.1f, 100.f, glm::radians(90.f), 0.f, { 0.f, 10.f, 0.f }, { glm::radians(90.f), 0.f, 0.f })
{
    // Events
    auto& gameLayer = vrm::Application::Get().getGameLayer();
    gameLayer.createTrigger("MoveForward")
        .bindInput(vrm::KeyCode::W)
        .bindCallback([this](bool triggered) { forwardValue += triggered ? 1.f : -1.f; });
    gameLayer.createTrigger("MoveBackward")
        .bindInput(vrm::KeyCode::S)
        .bindCallback([this](bool triggered) { forwardValue -= triggered ? 1.f : -1.f; });
    gameLayer.createTrigger("MoveRight")
        .bindInput(vrm::KeyCode::D)
        .bindCallback([this](bool triggered) { rightValue += triggered ? 1.f : -1.f; });
    gameLayer.createTrigger("MoveLeft")
        .bindInput(vrm::KeyCode::A)
        .bindCallback([this](bool triggered) { rightValue -= triggered ? 1.f : -1.f; });
    gameLayer.createTrigger("MoveUp")
        .bindInput(vrm::KeyCode::Space)
        .bindCallback([this](bool triggered) { upValue += triggered ? 1.f : -1.f; });
    gameLayer.createTrigger("MoveDown")
        .bindInput(vrm::KeyCode::LeftShift)
        .bindCallback([this](bool triggered) { upValue -= triggered ? 1.f : -1.f; });
    gameLayer.createTrigger("MouseLeft")
        .bindInput(vrm::MouseCode::Left);
    
    gameLayer.createCustomEvent("MouseMoved")
        .bindInput(vrm::Event::Type::MouseMoved)
        .bindCallback([this](const vrm::Event& event) {
            turnRightValue += static_cast<float>(event.mouseDeltaX);
            lookUpValue -= static_cast<float>(event.mouseDeltaY);
        });

    // Heigh field setup
    vrm::Texture2D tex;
    VRM_ASSERT(tex.loadFromFile("Resources/Textures/great_lakes.jpg"));
    m_HeightField.setFromTexture(tex);
}

HeightFieldScene::~HeightFieldScene()
{
}

void HeightFieldScene::onInit()
{
    // Camera
    setCamera(&m_Camera);

    // Entities
    auto meshEntity = createEntity("HeightField");
    meshEntity.addComponent<vrm::MeshComponent>(m_MeshAsset.createInstance());

    auto lightEntity = createEntity("Light");
        auto& lightTransform = lightEntity.getComponent<vrm::TransformComponent>();
            lightTransform.setPosition({0.f, 100.f, 0.f});
        m_LightComponent = &lightEntity.addComponent<vrm::PointLightComponent>();
            m_LightComponent->color = glm::vec3{1.f};
            m_LightComponent->intensity = 4'000.f;
            m_LightComponent->radius = 10'000.f;

    updateMesh();
}

void HeightFieldScene::onEnd()
{
}

void HeightFieldScene::onUpdate(float dt)
{
    /* Camera */
    m_Camera.move(forwardValue * myCameraSpeed * dt * m_Camera.getForwardVector());
    m_Camera.move(rightValue * myCameraSpeed * dt * m_Camera.getRightVector());
    m_Camera.move(upValue * myCameraSpeed * dt * glm::vec3{0.f, 1.f, 0.f});
    m_Camera.addYaw(turnRightValue * myCameraAngularSpeed);
    m_Camera.addPitch(lookUpValue * myCameraAngularSpeed);

    lookUpValue = 0.f;
    turnRightValue = 0.f;
}

void HeightFieldScene::onRender()
{
}

void HeightFieldScene::updateMesh()
{
    m_MeshAsset.clear();
    m_MeshAsset.addSubmesh(m_HeightField.toMeshData(10.f));
    VRM_LOG_INFO("Mesh updated");
}