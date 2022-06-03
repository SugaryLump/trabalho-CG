#include "engine/textures.hpp"

#include <string>
#include <IL/il.h>

void initIL() {
    ilInit();
}

Texture::Texture(std::string textureName) {
    unsigned int image;

    ilGenImages(1, &image);
    ilBindImage(image);
    ilLoadImage(textureName.c_str());
    width = ilGetInteger(IL_IMAGE_WIDTH);
    height = ilGetInteger(IL_IMAGE_HEIGHT);
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    imageData = ilGetData();
}