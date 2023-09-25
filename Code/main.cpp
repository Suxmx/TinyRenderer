#include <iostream>
#include "ThirdParties/geometry.h"
#include "ThirdParties/tgaimage.h"
#include"model.h"
#include "renderer.h"
#include "utilities.h"
#include "Texture.h"
#include "rasterizer.h"
#include"Camera.h"
#include <functional>

using namespace std;

const TGAColor red = TGAColor({0, 0, 255, 255});
const TGAColor white = TGAColor({255, 255, 255, 255});
const TGAColor blue = TGAColor({255, 0, 0, 255});

int main() {
    TGAImage image(getWidth(), getHeight(), TGAImage::RGB);
    Model test("african_head.obj");
    Texture texture("african_head_diffuse.tga");
    NormalTexture normalTexture("african_head_nm_tangent.tga");
    rasterizer r(500, 500);
    Shader *shader = new DiffuseShader();
    vector<faceInfo> *faces;
    //封装Shader
    std::function<vec3(vertexShaderPayload &payload)> vertexShader = [shader](vertexShaderPayload &payload) {
        return shader->vertexShader(payload);
    };
    std::function<TGAColor(fragShaderPayload &)> fragShader = [shader](fragShaderPayload &payload) {
        return shader->fragShader(payload);
    };
    faces = test.getAllFaces();
    //设置mvp矩阵
    mat<4, 4> modelMat, viewMat, projMat;
    Camera camera(vec3(0,0,0),vec3(0,0,1));
    modelMat = mat<4, 4>::identity();
    viewMat = camera.getViewMat();
    projMat = mat<4, 4>::identity();;
//    projMat[3][2] = -1. / 4;
    r.setModelMat(modelMat);
    r.setViewMat(viewMat);
    r.setProjMat(projMat);
    //设置贴图模型着色器
    r.setTexture(texture);
    r.setShader(shader);
    //DrawCall
    r.draw(*faces);

    shader = new PhongShader();
    r.setShader(shader);
    r.draw(*faces);

    shader = new TestShader();
    r.setShader(shader);
    r.draw(*faces);

    shader = new NormalMappingShader();
    r.setShader(shader);
    r.setNormalTexture(normalTexture);
    r.draw(*faces);
    return 0;
}

