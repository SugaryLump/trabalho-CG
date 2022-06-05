#pragma once

#include "common/geometry.hpp"

class LightSource {
   public:
    LightSource() = default;
    virtual void setupLight(int index) {}
};

class PointLight:public LightSource {
   public:
    Vector3 position;

    PointLight(Vector3 position);
    void setupLight(int index) override;
};

class DirectionalLight:public LightSource {
   public:
    Vector3 direction;

    DirectionalLight(Vector3 direction);
    void setupLight(int index) override;
};

class Spotlight:public LightSource {
   public:
    Vector3 position;
    Vector3 direction;
    float cutoff;

    Spotlight(Vector3 position, Vector3 direction, float cutoff);
    void setupLight(int index) override;
};

class AmbientLight:public LightSource {
   public:
    Vector3 intensity;

    AmbientLight(Vector3 intensity);
    void setupLight(int index) override;
};