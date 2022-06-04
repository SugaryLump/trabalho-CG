#include "engine/xmlParser.hpp"

#include <fmt/core.h>
#include "engine/light.hpp"
#include "engine/config.hpp"

#include <iostream>
#include <memory>
#include <optional>
#include <pugixml.hpp>
using namespace std;

map<string, Model> modelTable;
unordered_set<string> textureNames;

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
            for (pugi::xml_node node : node_models) {
                string name = node.name();
                if (name == "model") {
                    std::string modelName = node.attribute("file").value();
                    if (modelTable.find(modelName) == modelTable.end()) {
                        Model modelObject = Model(modelName);
                        modelTable[modelName] = modelObject;
                    }
                    ColorData colorData = ColorData();
                    std::string textureName = "";

                    for (pugi::xml_node modelNode: node) {
                        string name = modelNode.name();
                        if (name == "texture") {
                            textureName = node.attribute("file").value();
                            if (textureNames.find(textureName) == textureNames.end()) {
                                textureNames.insert(textureName);
                            }
                        }
                        else if (name == "color") {

                            for (pugi::xml_node colorNode : modelNode) {
                                string name = colorNode.name();
                                if (name != "shininess") {
                                    float cr = stof(node.attribute("R").value());
                                    float cg = stof(node.attribute("G").value());
                                    float cb = stof(node.attribute("B").value());
                                    Vector3 cv = Vector3(cr, cg, cb);
                                    if (name == "diffuse") {
                                        colorData.diffuse = cv;
                                    }
                                    else if (name == "ambient") {
                                        colorData.ambient = cv;
                                    }
                                    else if (name == "specular") {
                                        colorData.specular = cv;
                                    }
                                    else if (name == "emissive") {
                                        colorData.emissive = cv;
                                    }
                                }
                                else {
                                    colorData.shininess = stof(node.attribute("shininess").value());
                                }
                            }
                        }
                    }

                    ModelContainer model = ModelContainer(modelName, textureName, colorData);
                    model_group.rootModels.push_back(model);
                }
            }
        } else if (name == "group") {
            model_group.addChildGroup(parseModel(node_models));
        }
    }
    return model_group;
}

LightSource parseLight(pugi::xml_node node) noexcept {
    LightSource light;

    std::string type = node.attribute("type").value();
    if (type == "point") {
        float posx, posy, posz;
        posx = stof(node.attribute("posX").value());
        posy = stof(node.attribute("posY").value());
        posz = stof(node.attribute("posZ").value());
        light = PointLight(Vector3(posx, posy, posz));
    }
    else if (type == "directional") {
        float dirx, diry, dirz;
        dirx = stof(node.attribute("dirX").value());
        diry = stof(node.attribute("dirY").value());
        dirz = stof(node.attribute("dirZ").value());
        light = DirectionalLight(Vector3(dirx, diry, dirz));
    }
    else if (type == "spotlight") {
        float posx, posy, posz, dirx, diry, dirz, cutoff;
        posx = stof(node.attribute("posX").value());
        posy = stof(node.attribute("posY").value());
        posz = stof(node.attribute("posZ").value());
        dirx = stof(node.attribute("dirX").value());
        diry = stof(node.attribute("dirY").value());
        dirz = stof(node.attribute("dirZ").value());
        cutoff = stof(node.attribute("cutoff").value());
        light = Spotlight(Vector3(posx, posy, posz), Vector3(dirx, diry, dirz), cutoff);
    }

    return light;
}

optional<Config> parser(const std::string& filename) noexcept {
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
            c.models.push_back(parseModel(node));
        } else if (name == "light") {
            c.lights.push_back(parseLight(node));
        }
    }
    c.modelTable = modelTable;
    c.textureNames = textureNames;

    return c;
}
}  // namespace Parser
