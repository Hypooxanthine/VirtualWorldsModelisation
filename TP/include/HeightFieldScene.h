#pragma once

#include <Vroom/Scene/Scene.h>
#include <Vroom/Asset/StaticAsset/MeshAsset.h>
#include <Vroom/Scene/Components/PointLightComponent.h>
#include <Vroom/Scene/Components/TransformComponent.h>

#include "ScalarFields/HeightField.h"

class HeightFieldScene final : public vrm::Scene
{
public:
    HeightFieldScene();
    ~HeightFieldScene();

    void onInit() override;
    void onEnd() override;
    void onUpdate(float dt) override;
    void onRender() override;

    void reloadTexture(const std::string& path);
    void updateHeightField(const ScalarField::FromTextureSpecs& specs, bool shouldUpdateMesh = true);
    void updateMesh();

    inline vrm::ByteTextureData& getHeightMap() { return m_HeightMap; }
    inline HeightField& getHeightField() { return m_HeightField; }
    inline vrm::TransformComponent& getMeshTransform() { return *m_MeshTransform; }
    inline vrm::PointLightComponent& getLightComponent() { return *m_LightComponent; }
    inline vrm::TransformComponent& getLightTransform() { return *m_LightTransform; }

private:
    vrm::ByteTextureData m_HeightMap;
    HeightField m_HeightField;
    vrm::MeshAsset m_MeshAsset;
    vrm::TransformComponent* m_MeshTransform = nullptr;

    // Light params
    vrm::PointLightComponent* m_LightComponent = nullptr;
    vrm::TransformComponent* m_LightTransform = nullptr;
};