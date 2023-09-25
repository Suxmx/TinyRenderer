#ifndef MYRENDERER_SHADER_H
#define MYRENDERER_SHADER_H

#include "../ThirdParties/geometry.h"
#include"../Texture.h"
#include <vector>

struct vertexShaderPayload {
    vec3 pos;
    mat<4, 4> modelMat, viewMat, projMat;
    vec3 ndcPos;
    vertexShaderPayload(vec3 pos, mat<4, 4> &modelMat, mat<4, 4> &viewMat, mat<4, 4> &projMat) : pos(pos),
                                                                                                 modelMat(modelMat),
                                                                                                 viewMat(viewMat),
                                                                                                 projMat(projMat) {}
};

struct fragShaderPayload {
    fragShaderPayload() {
        texture = nullptr;
        normalTexture = nullptr;
    }

    fragShaderPayload(const vec3 &normal, const vec3 &uv, Texture *texture) :
            normal(normal),
            uv(uv),
            texture(texture) { normalTexture = nullptr; }

    vec3 viewPos;
    vec3 normal;
    vec3 uv;
    Texture *texture;
    NormalTexture *normalTexture;

};

class Shader {
public:
    virtual vec3 vertexShader(vertexShaderPayload &payload);

    virtual TGAColor fragShader(fragShaderPayload &payload) = 0;

    std::vector<vec3> vertexs;
protected:


};

class PhongShader : public Shader {
public:
    TGAColor fragShader(fragShaderPayload &payload) override;
};

class DiffuseShader : public Shader {
    TGAColor fragShader(fragShaderPayload &payload) override;
};

class TestShader : public Shader {
    TGAColor fragShader(fragShaderPayload &payload) override;
};
class NormalMappingShader: public Shader{
    TGAColor fragShader(fragShaderPayload &payload) override;
};

#endif //MYRENDERER_SHADER_H
