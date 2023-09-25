//
// Created by 42450 on 2023/9/16.
//
#include <iostream>
#include "Shader.h"
#include<cmath>

vec3 Shader::vertexShader(vertexShaderPayload &payload) {
    vec4 hc;
    for (int i = 0; i < 3; i++) {
        hc[i] = payload.pos[i];
    }
    hc[3] = 1;
    hc = payload.projMat * payload.viewMat * payload.modelMat * hc;
    for (int i = 0; i < 3; i++)
        payload.pos[i] = hc[i] / hc[3];
    payload.ndcPos = payload.pos;
    return vec3(hc[0], hc[1], hc[2]);
}

TGAColor PhongShader::fragShader(fragShaderPayload &payload) {
    vec3 lightDir(0, 0, 1);
    float intensity = lightDir * payload.normal;
    if (intensity < 0)return TGAColor(0, 0, 0, 0);
    TGAColor color(255 * intensity, 255 * intensity, 255 * intensity, 255);
//    std::cout<<static_cast<int>(color.r)<<" "<<static_cast<int>(color.g)<<" "<<static_cast<int>(color.b)<<std::endl;
//    std::cout<<"return:"<<  static_cast<int>(color.r)<<" "<<static_cast<int>(color.g)<<" "<<static_cast<int>(color.b)<<"\n";
    return color;
}


TGAColor DiffuseShader::fragShader(fragShaderPayload &payload) {
    assert(payload.texture != nullptr);
    vec3 lightDir(0, 0, 1);
    float intensity = lightDir * payload.normal;
    if (intensity < 0)return TGAColor(0, 0, 0, 0);
    TGAColor color = payload.texture->getColor(payload.uv[0], payload.uv[1]);
    return color * intensity;
}

TGAColor TestShader::fragShader(fragShaderPayload &payload) {
    vec3 lightDir(0, 0, 1);
    float intensity = lightDir * payload.normal;
    if (intensity > .85) intensity = 1;
    else if (intensity > .60) intensity = .80;
    else if (intensity > .45) intensity = .60;
    else if (intensity > .30) intensity = .45;
    else if (intensity > .15) intensity = .30;
    else intensity = 0;
    TGAColor color = TGAColor(255, 155, 0, 255) * intensity;
    return color;
}

TGAColor NormalMappingShader::fragShader(fragShaderPayload &payload) {
    vec3 lightDir(0, 0, 1);
    vec3 T, B, N, E1, E2;
    E1 = vertexs[1] - vertexs[0];
    E2 = vertexs[2] - vertexs[0];
    float du1 = vertexs[1][0] - vertexs[0][0], du2 = vertexs[2][0] - vertexs[0][0];
    float dv1 = vertexs[1][1] - vertexs[0][1], dv2 = vertexs[2][1] - vertexs[0][1];
    T = (E1 * dv2 - E2 * dv1) / (du1 * dv2 - du2 * dv1);
    T=T.normalized();
    B = (E2 * du1 - E1 * du2) / (du1 * dv2 - du2 * dv1);
    B=B.normalized();
//    T = T - (T * payload.normal) * payload.normal;
//    B = B - (B * payload.normal) * payload.normal - (B * T) * T;
    mat<3, 3> tbn;
    tbn.set_col(0, T);
    tbn.set_col(1, B);
    tbn.set_col(2, payload.normal);
    vec3 normal = payload.normalTexture->getNorm(payload.uv[0], payload.uv[1]);
    normal = tbn * normal;
    float intensity = lightDir * normal;
    if (intensity < 0)return TGAColor(0, 0, 0, 0);
    TGAColor color = payload.texture->getColor(payload.uv[0], payload.uv[1]) * intensity;
//    std::cout << static_cast<int>(color.r) << " " << static_cast<int>(color.g) << " " << static_cast<int>(color.b)
//              << std::endl;
    return color;
}
