#pragma once

#include <Vroom/Scene/Scene.h>
#include <Vroom/Asset/StaticAsset/MeshAsset.h>
#include <Vroom/Render/Camera/FirstPersonCamera.h>
#include <Vroom/Scene/Components/PointLightComponent.h>
#include <Vroom/Scene/Components/TransformComponent.h>
#include <glm/gtc/constants.hpp>

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

    void updateMesh();

    inline vrm::TransformComponent& getMeshTransform() { return *m_MeshTransform; }
    inline vrm::PointLightComponent& getLightComponent() { return *m_LightComponent; }
    inline vrm::TransformComponent& getLightTransform() { return *m_LightTransform; }

private:
    HeightField m_HeightField;
    vrm::MeshAsset m_MeshAsset;
    vrm::TransformComponent* m_MeshTransform = nullptr;

    // Camera params
    vrm::FirstPersonCamera m_Camera;
    float forwardValue = 0.f, rightValue = 0.f, upValue = 0.f;
	float turnRightValue = 0.f, lookUpValue = 0.f;
	float myCameraSpeed = 10.f, myCameraAngularSpeed = .08f * glm::two_pi<float>() / 360.f;

    // Light params
    vrm::PointLightComponent* m_LightComponent = nullptr;
    vrm::TransformComponent* m_LightTransform = nullptr;
};