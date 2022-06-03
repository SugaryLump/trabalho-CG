#pragma once

#include "common/geometry.hpp"

class LightSource {
   public:
    LightSource() = default;
    virtual void setupLight(int index) {}
};

class PointLight:LightSource {
   public:
    Vector3 position;

    PointLight(Vector3 position);
    void setupLight(int index) override;
};

class DirectionalLight:LightSource {
   public:
    Vector3 direction;

    DirectionalLight(Vector3 direction);
    void setupLight(int index) override;
};

class Spotlight:LightSource {
   public:
    Vector3 position;
    Vector3 direction;
    float cutoff;

    Spotlight(Vector3 position, Vector3 direction, float cutoff);
    void setupLight(int index) override;
};