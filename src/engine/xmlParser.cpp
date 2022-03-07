#include "engine/xmlParser.hpp"
#include "engine/camera.hpp"
#include "dependencies/pugixml.hpp"
#include <iostream>

using namespace std;

namespace Parser{

    Camera parseCamera(pugi::xml_node node) {
        pugi::xml_node pos = node.child("position");

        float x = stof(pos.attribute("x").value());
        float y = stof(pos.attribute("y").value());
        float z = stof(pos.attribute("z").value());
        // fazer cenas

        pos = node.child("lookAt");
        x = stof(pos.attribute("x").value());
        y = stof(pos.attribute("y").value());
        z = stof(pos.attribute("z").value());
        // fazer cenas

        pos = node.child("up");
        x = stof(pos.attribute("x").value());
        y = stof(pos.attribute("y").value());
        z = stof(pos.attribute("z").value());
        // fazer cenas


        pos = node.child("projection");
        float near = stof(pos.attribute("near").value());
        float far = stof(pos.attribute("far").value());
        float fov = stof(pos.attribute("fov").value());
        // fazer cenas

        Camera* c = new Camera(near, far, fov);
        // todo set position, lookant and up

        return *c;
    }

    int parser() {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file("/home/pasok/trabalho-CG/build/src/engine/config.xml");
        if (!result) {
            std::cout << "erro: " << result;
            return -1;
       }
        //std::cout << doc.child("world").child("group") << std::endl;

        Camera c;
        for (pugi::xml_node node: doc.child("world") ) {
            string name = node.name();
            if(!name.compare("camera")) {
                parseCamera(node);
            }
            //else if(!name.compare("group")) {
            //}
        }
        return 0;
    }
}
