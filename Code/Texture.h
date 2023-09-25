#ifndef MYRENDERER_TEXTURE_H
#define MYRENDERER_TEXTURE_H

#include"ThirdParties/tgaimage.h"
#include"ThirdParties/geometry.h"

class Texture {
protected:
    TGAImage image;
public:
    Texture(const char *filename);

    int width;
    int height;

    virtual TGAColor getColor(float u, float v);
};

class NormalTexture : public Texture {
public:
    vec3 getNorm(float u, float v);

    NormalTexture(const char *filename);
};

#endif //MYRENDERER_TEXTURE_H
