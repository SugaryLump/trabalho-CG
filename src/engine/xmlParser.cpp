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
                    pugi::xml_attribute timeAttribute = node.attribute("time");
                    if (timeAttribute) {
                        float seconds = stof(timeAttribute.value());
                        bool align = strcmp(node.attribute("align").value(), "False");
                        std::vector<std::vector<float>> points;
                        for (pugi::xml_node node_translate : node) {
                            string name = node_translate.name();
                            if (name == "point") {
                                float x = stof(node_translate.attribute("x").value());
                                float y = stof(node_translate.attribute("y").value());
                                float z = stof(node_translate.attribute("z").value());
                                std::vector<float> point;
                                point.push_back(x);
                                point.push_back(y);
                                point.push_back(z);
                                points.push_back(point);
                            }
                        }
                        shared_ptr<Curve> curve = make_shared<Curve>(points, points.size(), seconds, align);
                        model_group.transformations.push_back(curve);
                    }
                    else {
                        float x = stof(node.attribute("x").value());
                        float y = stof(node.attribute("y").value());
                        float z = stof(node.attribute("z").value());
                        shared_ptr<Translate> translate = make_shared<Translate>(Vector3(x, y, z));
                        model_group.transformations.push_back(translate);
                    }
                } else if (name == "rotate") {
                    float x = stof(node.attribute("x").value());
                    float y = stof(node.attribute("y").value());
                    float z = stof(node.attribute("z").value());
                    pugi::xml_attribute angleAttribute = node.attribute("angle");
                    if (angleAttribute) {
                        float angle = stof(angleAttribute.value());
                        shared_ptr<Rotate> rotate = make_shared<Rotate>(Vector3(x, y, z), angle);
                        model_group.transformations.push_back(rotate);
                    }
                    else {
                        float time = stof(node.attribute("time").value());
                        shared_ptr<TimedRotate> timedRotate = make_shared<TimedRotate>(Vector3(x, y, z), time);
                        model_group.transformations.push_back(timedRotate);
                    }
                } else if (name == "scale") {
                    float x = stof(node.attribute("x").value());
                    float y = stof(node.attribute("y").value());
                    float z = stof(node.attribute("z").value());
                    shared_ptr<Scale> scale = make_shared<Scale>(Vector3(x, y, z));
                    model_group.transformations.push_back(scale);
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
            //c.models = parseModels(node);
            c.models.push_back(parseModel(node));
        }
    }
    return c;
}
}  // namespace Parser
