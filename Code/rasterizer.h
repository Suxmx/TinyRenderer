//
// Created by 42450 on 2023/9/16.
//

#ifndef MYRENDERER_RASTERIZER_H
#define MYRENDERER_RASTERIZER_H

#include "ThirdParties/geometry.h"
#include"ThirdParties/tgaimage.h"
#include"Shaders/Shader.h"
#include"model.h"
#include "Texture.h"
#include "utilities.h"
#include"Buffer.h"
#include <vector>
#include<functional>
#include <optional>

class rasterizer {
private:
    TGAImage image;
    optional<Texture> texture;
    optional<NormalTexture> normTexture;
    mat<4, 4> modelMat;
    mat<4, 4> viewMat;
    mat<4, 4> projMat;
    std::function<vec3(vertexShaderPayload &payload)> vertexShader;
    std::function<TGAColor(fragShaderPayload &payload)> fragShader;
    Shader *shader= nullptr;
    int width, height;
    int drawNums = 0;

public:
    rasterizer(int width, int height) {
        this->image = TGAImage(width, height, TGAImage::RGB);
        texture = nullopt;
        normTexture = nullopt;
        this->width = width;
        this->height = height;
    }

    void setTexture(Texture &texture);

    void setModelMat(mat<4, 4> &_modelMat);

    void setViewMat(mat<4, 4> &_viewMat);

    void setProjMat(mat<4, 4> &_projMat);

    void setShader(Shader *shader);

    void setVertexShader(std::function<vec3(vertexShaderPayload &payload)> &vertexShader);

    void setFragShader(std::function<TGAColor(fragShaderPayload &payload)> &fragShader);

    void draw(const vector<faceInfo> &faces);

    void clear();

    void setNormalTexture(NormalTexture &norm);

    rasterizer();
};


#endif //MYRENDERER_RASTERIZER_H
