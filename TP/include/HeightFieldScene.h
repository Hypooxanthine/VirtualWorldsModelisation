#pragma once

#include <Vroom/Scene/Scene.h>

#include "ScalarFields/ScalarField.h"

class HeightFieldScene final : public vrm::Scene
{
public:
    HeightFieldScene();
    ~HeightFieldScene();

    void onInit() override;
    void onEnd() override;
    void onUpdate(float dt) override;
    void onRender() override;

private:
    ScalarField m_ScalarField;
};