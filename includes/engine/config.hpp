#pragma once

#include <memory>

#include "common/geometry.hpp"
#include "engine/camera.hpp"

struct Config {
    std::unique_ptr<Camera> camera;
    std::vector<Model> models;

    Config();
};