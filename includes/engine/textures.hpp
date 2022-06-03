#pragma once

#include <string>

void initIL();

class TextureData {
   public:
    unsigned char* imageData;
    unsigned int width;
    unsigned int height;

    TextureData() = default;
    TextureData(std::string textureName);
};