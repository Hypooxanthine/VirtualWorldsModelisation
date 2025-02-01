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

    void highlightPoint(size_t x, size_t y, float radius = 1.f);
    void enableHighlightSingle(bool enable);

    void highlightStartPath(size_t x, size_t y);
    void highlightEndPath(size_t x, size_t y);

    void startPathAnimation(const std::vector<int>& path);
    void stopPathAnimation();
    inline void setPathAnimationTime(float time) { m_PathAnimationTime = time; }

    inline vrm::ByteTextureData& getHeightMap() { return m_HeightMap; }
    inline HeightField& getHeightField() { return m_HeightField; }
    inline vrm::TransformComponent& getMeshTransform() { return *m_MeshTransform; }
    inline vrm::PointLightComponent& getLightComponent() { return *m_LightComponent; }
    inline vrm::TransformComponent& getLightTransform() { return *m_LightTransform; }

private:
    void hightlightPath(size_t index);

private:
    vrm::ByteTextureData m_HeightMap;
    HeightField m_HeightField;
    vrm::MeshAsset m_MeshAsset;
    vrm::TransformComponent* m_MeshTransform = nullptr;

    // Light params
    vrm::PointLightComponent* m_LightComponent = nullptr;
    vrm::TransformComponent* m_LightTransform = nullptr;

    // Highlighting
    bool m_IsAnimatingPath = false;
    std::vector<int> m_Path;
    size_t m_PathIndex = 0;
    float m_PathAnimationTime = 0.f;
    float m_TimeSinceLastIndex = 0.f;
};