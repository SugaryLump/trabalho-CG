#include "engine/xmlParser.hpp"

#include <iostream>
#include <pugixml.hpp>

using namespace std;

namespace Parser {

Camera parseCamera(pugi::xml_node node) {
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

    Camera c = Camera(look, up, position, near, far, fov);
    return c;
}

std::vector<Model> parseModels(pugi::xml_node node) {
    std::vector<Model> models;
    for (pugi::xml_node node : node.child("models")) {
        string name = node.name();
        if (!name.compare("model")) {
            const char* filename = node.attribute("file").value();
            models.push_back(Model(filename));
        }
    }
    return models;
}

Config parser(char* filename) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename);
    if (!result) {
        std::cout << "erro: " << result << "\n";
        // FIXME CHANGE THIS
        throw "error";
        // return -1;
    }
    // std::cout << "Read config.xml\n ";

    Config c = Config();
    // std::cout << doc.child("world").child("group") << std::endl;
    // Camera c;
    for (pugi::xml_node node : doc.child("world")) {
        string name = node.name();
        if (!name.compare("camera")) {
            // FIXME dar free da camera default q ja tinha
            c.camera = parseCamera(node);
        } else if (!name.compare("group")) {
            try {
                c.models = parseModels(node);
            } catch (exception const& e) { std::cout << e.what() << "\n"; }
            // std::cout << "Li X modelos :: " << c.models.size() << endl;
            // std::cout << c.models[0].vertices.size() << "\n";
            // std::cout << c.models[0].indices.size() << "\n";
        }
    }
    return c;
}
}  // namespace Parser
