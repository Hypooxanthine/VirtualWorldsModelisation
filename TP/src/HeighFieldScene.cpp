#include "HeightFieldScene.h"

#include <Vroom/Core/Application.h>
#include <Vroom/Core/GameLayer.h>
#include <Vroom/Scene/Components/MeshComponent.h>

HeightFieldScene::HeightFieldScene()
{
    // Heigh field setup
    vrm::Texture2D tex;
    VRM_ASSERT_MSG(tex.loadFromFile("Resources/Textures/great_lakes.jpg"), "Failed to load texture");
    m_HeightField.setFromTexture(tex);
}

HeightFieldScene::~HeightFieldScene()
{
}

void HeightFieldScene::onInit()
{

    // Entities
    auto meshEntity = createEntity("HeightField");
    meshEntity.addComponent<vrm::MeshComponent>(m_MeshAsset.createInstance());
    m_MeshTransform = &meshEntity.getComponent<vrm::TransformComponent>();
        m_MeshTransform->setScale({1.f, .025f, 1.f});

    auto lightEntity = createEntity("Light");
        m_LightTransform = &lightEntity.getComponent<vrm::TransformComponent>();
            m_LightTransform->setPosition({0.f, 100.f, 0.f});
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