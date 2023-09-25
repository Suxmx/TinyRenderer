//
// Created by 42450 on 2023/9/16.
//

#include "Texture.h"

Texture::Texture(const char *filename) {
    image.read_tga_file(filename);
    width = image.get_width();
    height = image.get_height();
    image.flip_vertically();
}


TGAColor Texture::getColor(float u, float v) {
    return image.get(u * width, v * height);
}


vec3 NormalTexture::getNorm(float u, float v) {
    TGAColor color = image.get(u * width, v * height);
    vec3 norm;
    norm.x = (static_cast<int>(color.r) / 255.0) * 2 - 1;
    norm.y = (static_cast<int>(color.g) / 255.0) * 2 - 1;
    norm.z = (static_cast<int>(color.b) / 255.0) * 2 - 1;
    return norm;
}

NormalTexture::NormalTexture(const char *filename) : Texture(filename) {
    image.read_tga_file(filename);
    width = image.get_width();
    height = image.get_height();
    image.flip_vertically();
}
