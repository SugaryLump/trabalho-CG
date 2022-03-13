#include "engine/config.hpp"

Config::Config() {
    camera = std::make_unique<Camera>();
}