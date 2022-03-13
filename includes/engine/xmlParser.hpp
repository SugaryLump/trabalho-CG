#pragma once

#include <optional>

#include "engine/camera.hpp"
#include "engine/config.hpp"

namespace Parser {
std::optional<Config> parser(const std::string& filename) noexcept;
}