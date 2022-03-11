#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include "common/geometry.hpp"
#include "engine/camera.hpp"

struct Config {
    Camera camera;
    std::vector<Model> models;

    Config();
};

#endif  // Config_HPP_