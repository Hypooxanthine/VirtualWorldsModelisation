#include "HeightFieldScene.h"

#include <Vroom/Core/Application.h>
#include <Vroom/Core/GameLayer.h>
#include <Vroom/Scene/Components/MeshComponent.h>

HeightFieldScene::HeightFieldScene()
{
}

HeightFieldScene::~HeightFieldScene()
{
    std::filesystem::remove_all("Temp");
}

void HeightFieldScene::onInit()
{
    // Entities
    auto meshEntity = createEntity("HeightField");
    meshEntity.addComponent<vrm::MeshComponent>(m_MeshAsset.createInstance());
    m_MeshTransform = &meshEntity.getComponent<vrm::TransformComponent>();

    auto lightEntity = createEntity("Light");
        m_LightTransform = &lightEntity.getComponent<vrm::TransformComponent>();
            m_LightTransform->setPosition({25.f, 50.f, 25.f});
        m_LightComponent = &lightEntity.addComponent<vrm::PointLightComponent>();
            m_LightComponent->color = glm::vec3{1.f};
            m_LightComponent->intensity = 1'000.f;
            m_LightComponent->radius = 10'000.f;
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

void HeightFieldScene::reloadTexture(const std::string& path)
{
    VRM_ASSERT_MSG(m_HeightMap.loadFromFile(path), "Failed to load texture");
}

void HeightFieldScene::updateHeightField(const ScalarField::FromTextureSpecs& specs, bool shouldUpdateMesh)
{
    m_HeightField.setFromTexture(m_HeightMap, specs);

    if (shouldUpdateMesh)
        updateMesh();
}

void HeightFieldScene::updateMesh()
{
    m_MeshAsset.clear();
    m_MeshAsset.addSubmesh(m_HeightField.toMeshData());
    VRM_LOG_INFO("Mesh updated");
}
