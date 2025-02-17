#include "HeightFieldScene.h"

#include <Vroom/Core/Application.h>
#include <Vroom/Core/GameLayer.h>
#include <Vroom/Asset/AssetManager.h>
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
        m_MeshTransform = &meshEntity.getComponent<vrm::TransformComponent>();
        meshEntity.addComponent<vrm::MeshComponent>(m_MeshAsset.createInstance());

    auto lightEntity = createEntity("Light");
        m_LightTransform = &lightEntity.getComponent<vrm::TransformComponent>();
            m_LightTransform->setPosition({25.f, 50.f, 25.f});
        m_LightComponent = &lightEntity.addComponent<vrm::PointLightComponent>();
            m_LightComponent->color = glm::vec3{1.f};
            m_LightComponent->intensity = 1'000.f;
            m_LightComponent->radius = 10'000.f;

    auto highlightSingleEntity = createEntity("highlightSingle");
        highlightSingleEntity.addComponent<vrm::MeshComponent>(
            vrm::AssetManager::Get().getAsset<vrm::MeshAsset>("Resources/Meshes/ControlPoint.obj")
        ).setVisible(false);
        highlightSingleEntity.getComponent<vrm::TransformComponent>().setScale({ 1.f, 1.f, 1.f });

    auto highlightStartPathEntity = createEntity("highlightStartPath");
        highlightStartPathEntity.addComponent<vrm::MeshComponent>(
            vrm::AssetManager::Get().getAsset<vrm::MeshAsset>("Resources/Meshes/ControlPoint.obj")
        ).setVisible(false);
        highlightStartPathEntity.getComponent<vrm::TransformComponent>().setScale({ 1.f, 1.f, 1.f });
    
    auto highlightEndPathEntity = createEntity("highlightEndPath");
        highlightEndPathEntity.addComponent<vrm::MeshComponent>(
            vrm::AssetManager::Get().getAsset<vrm::MeshAsset>("Resources/Meshes/ControlPoint.obj")
        ).setVisible(false);
        highlightEndPathEntity.getComponent<vrm::TransformComponent>().setScale({ 1.f, 1.f, 1.f });

    auto hightlightPathEntity = createEntity("highlightPath");
        hightlightPathEntity.addComponent<vrm::MeshComponent>(
            vrm::AssetManager::Get().getAsset<vrm::MeshAsset>("Resources/Meshes/ControlPoint.obj")
        ).setVisible(false);
        hightlightPathEntity.getComponent<vrm::TransformComponent>().setScale({ 1.f, 1.f, 1.f });
}

void HeightFieldScene::onEnd()
{
}

void HeightFieldScene::onUpdate(float dt) {}

void HeightFieldScene::onEditorUpdate(float dt)
{
    if (m_IsAnimatingPath)
    {
        m_TimeSinceLastIndex += dt;
        float timePerIndex = m_PathAnimationTime / static_cast<float>(m_Path.size());
        if (m_TimeSinceLastIndex >= timePerIndex)
        {
            hightlightPath(m_Path[m_PathIndex]);

            int skip = static_cast<int>(m_TimeSinceLastIndex / timePerIndex);
            m_PathIndex += skip;
            m_PathIndex = m_PathIndex % m_Path.size();
            m_TimeSinceLastIndex -= (timePerIndex * skip);
        }
    }
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

void HeightFieldScene::setHighlightRadius(float radius)
{
    m_HighlightRadius = radius;
    glm::vec3 newRadius = { radius, radius, radius };

    getEntity("highlightSingle")
        .getComponent<vrm::TransformComponent>()
        .setScale(newRadius);
        
    getEntity("highlightStartPath")
        .getComponent<vrm::TransformComponent>()
        .setScale(newRadius);

    getEntity("highlightEndPath")
        .getComponent<vrm::TransformComponent>()
        .setScale(newRadius);

    getEntity("highlightPath")
        .getComponent<vrm::TransformComponent>()
        .setScale(newRadius);
}

void HeightFieldScene::highlightPoint(size_t x, size_t y)
{
    glm::vec3 localPos = m_HeightField.getLocalPosition(x, y);
    glm::vec3 worldPos = m_MeshTransform->getTransform() * glm::vec4(localPos, 1.f);

    auto e = getEntity("highlightSingle");
    auto& tc = e.getComponent<vrm::TransformComponent>();
    tc.setPosition(worldPos);
    tc.setScale({m_HighlightRadius, m_HighlightRadius, m_HighlightRadius});
    e.getComponent<vrm::MeshComponent>().setVisible(true);
}

void HeightFieldScene::enableHighlightSingle(bool enable)
{
    getEntity("highlightSingle").getComponent<vrm::MeshComponent>().setVisible(enable);
}

void HeightFieldScene::highlightStartPath(size_t x, size_t y)
{
    glm::vec3 localPos = m_HeightField.getLocalPosition(x, y);
    glm::vec3 worldPos = m_MeshTransform->getTransform() * glm::vec4(localPos, 1.f);

    auto e = getEntity("highlightStartPath");
    auto& tc = e.getComponent<vrm::TransformComponent>();
    tc.setPosition(worldPos);
    e.getComponent<vrm::MeshComponent>().setVisible(true);
}

void HeightFieldScene::highlightEndPath(size_t x, size_t y)
{
    glm::vec3 localPos = m_HeightField.getLocalPosition(x, y);
    glm::vec3 worldPos = m_MeshTransform->getTransform() * glm::vec4(localPos, 1.f);

    auto e = getEntity("highlightEndPath");
    auto& tc = e.getComponent<vrm::TransformComponent>();
    tc.setPosition(worldPos);
    e.getComponent<vrm::MeshComponent>().setVisible(true);
}

void HeightFieldScene::startPathAnimation(const std::vector<int>& path)
{
    m_Path = path;
    m_PathIndex = 0;
    m_IsAnimatingPath = true;

    getEntity("highlightPath").getComponent<vrm::MeshComponent>().setVisible(true);
}

void HeightFieldScene::stopPathAnimation()
{
    m_IsAnimatingPath = false;
    getEntity("highlightPath").getComponent<vrm::MeshComponent>().setVisible(false);
}

void HeightFieldScene::hightlightPath(size_t index)
{
    glm::vec3 localPos = m_HeightField.getLocalPosition(m_HeightField.xCoord(index), m_HeightField.yCoord(index));
    glm::vec3 worldPos = m_MeshTransform->getTransform() * glm::vec4(localPos, 1.f);

    auto e = getEntity("highlightPath");
    auto& tc = e.getComponent<vrm::TransformComponent>();
    tc.setPosition(worldPos);
}