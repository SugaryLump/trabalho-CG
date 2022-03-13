#include "engine/xmlParser.hpp"

#include <fmt/core.h>

#include <memory>
#include <optional>
#include <pugixml.hpp>

using namespace std;

namespace Parser {

std::unique_ptr<Camera> parseCamera(pugi::xml_node node) noexcept {
    pugi::xml_node pos = node.child("position");

    float x = stof(pos.attribute("x").value());
    float y = stof(pos.attribute("y").value());
    float z = stof(pos.attribute("z").value());
    Vector3 position = {x, y, z};

    pos = node.child("lookAt");
    x = stof(pos.attribute("x").value());
    y = stof(pos.attribute("y").value());
    z = stof(pos.attribute("z").value());
    Vector3 look = {x, y, z};

    pos = node.child("up");
    x = stof(pos.attribute("x").value());
    y = stof(pos.attribute("y").value());
    z = stof(pos.attribute("z").value());
    Vector3 up = {x, y, z};

    pos = node.child("projection");
    float near = stof(pos.attribute("near").value());
    float far = stof(pos.attribute("far").value());
    float fov = stof(pos.attribute("fov").value());

    auto c = std::make_unique<Camera>(look, up, position, near, far, fov);
    return c;
}

std::vector<Model> parseModels(pugi::xml_node node) noexcept {
    std::vector<Model> models;
    for (pugi::xml_node models_node : node.child("models")) {
        string name = models_node.name();
        if (name == "model") {
            const char* filename = models_node.attribute("file").value();
            models.emplace_back(filename);
        }
    }
    return models;
}

std::optional<Config> parser(const std::string& filename) noexcept {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename.c_str());

    if (!result) {
        fmt::print(stderr, "error: {}\n", result);
        return {};
    }

    Config c = Config();
    for (pugi::xml_node node : doc.child("world")) {
        string name = node.name();
        if (name == "camera") {
            c.camera = parseCamera(node);
        } else if (name == "group") {
            c.models = parseModels(node);
            // std::cout << "Li X modelos :: " << c.models.size() << endl;
            // std::cout << c.models[0].vertices.size() << "\n";
            // std::cout << c.models[0].indices.size() << "\n";
        }
    }
    return c;
}
}  // namespace Parser
