#include "engine/xmlParser.hpp"

#include <fmt/core.h>

#include <iostream>
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

ModelGroup parseModel(pugi::xml_node node) noexcept {
    ModelGroup model_group = ModelGroup();

    for (pugi::xml_node node_models : node) {
        string name = node_models.name();
        if (name == "transform") {
            for (pugi::xml_node node : node_models) {
                string name = node.name();
                if (name == "translate") {
                    float x = stof(node.attribute("x").value());
                    float y = stof(node.attribute("y").value());
                    float z = stof(node.attribute("z").value());
                    model_group.transformations.push_back(Transform(Transform::TRANSLATE, Vector3(x, y, z)));
                } else if (name == "rotate") {
                    float angle = stof(node.attribute("angle").value());
                    float x = stof(node.attribute("x").value());
                    float y = stof(node.attribute("y").value());
                    float z = stof(node.attribute("z").value());
                    model_group.transformations.push_back(Transform(Transform::ROTATE, Vector3(x, y, z), angle));
                } else if (name == "scale") {
                    float x = stof(node.attribute("x").value());
                    float y = stof(node.attribute("y").value());
                    float z = stof(node.attribute("z").value());
                    model_group.transformations.push_back(Transform(Transform::SCALE, Vector3(x, y, z)));
                }
            }
        } else if (name == "models") {
            for (pugi::xml_node node_models : node_models) {
                string name = node_models.name();
                if (name == "model") {
                    const char* filename = node_models.attribute("file").value();
                    model_group.rootModels.emplace_back(filename);
                }
            }
        } else if (name == "group") {
            model_group.addChildGroup(parseModel(node_models));
        }
    }
    return model_group;
}

std::vector<ModelGroup> parseModels(pugi::xml_node node) noexcept {
    std::vector<ModelGroup> models;
    models.push_back(parseModel(node));
    return models;
}

std::optional<Config> parser(const std::string& filename) noexcept {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename.c_str());
    std::vector<ModelGroup> models;

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
            // TODO
            c.models = parseModels(node);
            // c.models = parseModels(doc.child("world"));
            // c.models = models.push_back(parseModel(node));

            // std::cout << "Li X modelos :: " << c.models.size() << endl;
            // std::cout << c.models[0].vertices.size() << "\n";
            // std::cout << c.models[0].indices.size() << "\n";
        }
    }
    return c;
}
}  // namespace Parser
