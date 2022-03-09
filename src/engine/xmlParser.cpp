#include "engine/xmlParser.hpp"

#include <iostream>
#include <pugixml.hpp>

#include "engine/camera.hpp"
#include "engine/config.hpp"

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

    Camera c = Camera(up, position, look, near, far, fov);
    return c;
}

int parser() {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("/home/pasok/trabalho-CG/build/src/engine/config.xml");
    if (!result) {
        std::cout << "erro: " << result;
        return -1;
    }

    Config c;  // = Config::Config();
    // std::cout << doc.child("world").child("group") << std::endl;

    // Camera c;
    for (pugi::xml_node node : doc.child("world")) {
        string name = node.name();
        // FIXME Falta inicializar o config
        if (!name.compare("camera")) { c.camera = parseCamera(node); }
        // else if(!name.compare("group")) {
        // }
    }
    return 0;
}
}  // namespace Parser
