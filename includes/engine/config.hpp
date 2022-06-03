#pragma once

#include <memory>
#include <unordered_set>

#include "common/geometry.hpp"
#include "engine/camera.hpp"
#include "engine/light.hpp"

struct Config {
    std::unique_ptr<Camera> camera;
    std::vector<ModelGroup> models;
    std::map<std::string, Model> modelTable;
    std::unordered_set<std::string> textureNames;
    std::vector<LightSource> lights;

    Config();
};
