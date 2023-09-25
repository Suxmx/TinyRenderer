//
// Created by 42450 on 2023/9/11.
//
#ifndef MYRENDERER_UTILITIES_H
#define MYRENDERER_UTILITIES_H

#include "ThirdParties/geometry.h"
#include <vector>
#include"model.h"
#include "Shaders/Shader.h"
#include <tuple>

using namespace std;

bool insideTriangle(vec2 p, vec2 a, vec2 b, vec2 c);

bool insideTriangle(vec3 p, vec3 a, vec3 b, vec3 c);

vec3 vecRevolution(vec2 v, double z);

vec3 getBarycentric(vec3 A, vec3 B, vec3 C, vec3 P);
tuple<float,float,float> getBarycentric2D(vec3 A, vec3 B, vec3 C, vec3 P);
vec3 interpolate(float alpha,float beta,float gamma,vec3 A,vec3 B,vec3 C);

int getWidth();

int getHeight();

tuple<int, int, int, int> getBoundingBox(vector<vertexShaderPayload> &pay);

#endif //MYRENDERER_UTILITIES_H
