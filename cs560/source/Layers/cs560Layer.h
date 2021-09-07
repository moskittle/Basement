#pragma once

#include "Basement/Core/Layer.h"

class cs560Layer : public Basement::Layer
{
public:
    cs560Layer();
    ~cs560Layer() = default;


    virtual void Update(const Basement::Timer& dt) override;
    virtual void RenderImGui() override;
    virtual void HandleEvent(Basement::Event& event) override;
private:
    void BuildScene();
    void RenderScene();
};
