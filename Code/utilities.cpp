//
// Created by 42450 on 2023/9/11.
//
#include "utilities.h"
#include "Shaders/Shader.h"

int width = 500, height = 500;

vec3 vecRevolution(vec2 v, double z) {
    return vec3(v[0], v[1], z);
}

bool insideTriangle(vec2 p, vec2 a, vec2 b, vec2 c) {
    vec3 pa = vecRevolution((a - p), 0), ab = vecRevolution((b - a), 0);
    vec3 pb = vecRevolution((b - p), 0), bc = vecRevolution((c - b), 0);
    vec3 pc = vecRevolution((c - p), 0), ca = vecRevolution((a - c), 0);
    double crsa = cross(pa, ab)[2], crsb = cross(pb, bc)[2], crsc = cross(pc, ca)[2];
    return ((crsa > 0 && crsb > 0 && crsc > 0) || (crsa < 0 && crsb < 0 && crsc < 0));
}

bool insideTriangle(vec3 p, vec3 a, vec3 b, vec3 c) {
    vec3 pa = a - p, ab = b - a;
    vec3 pb = b - p, bc = c - b;
    vec3 pc = c - p, ca = a - c;
    double crsa = cross(pa, ab)[2], crsb = cross(pb, bc)[2], crsc = cross(pc, ca)[2];
    return ((crsa > 0 && crsb > 0 && crsc > 0) || (crsa < 0 && crsb < 0 && crsc < 0));
}

vec3 getBarycentric(vec3 A, vec3 B, vec3 C, vec3 P) {
    vec3 s[2];
    for (int i = 2; i--;) {
        s[i][0] = C[i] - A[i];
        s[i][1] = B[i] - A[i];
        s[i][2] = A[i] - P[i];
    }
    vec3 u = cross(s[0], s[1]);
    if (abs(u.z) > 1e-2) {
        u = u / u.z;
        return vec3(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
//        return u;
    }
    return vec3(-1, -1, -1);
}

int getWidth() {
    return width;
}

int getHeight() {
    return height;
}

tuple<int, int, int, int> getBoundingBox(vector<vertexShaderPayload> &pay) {

    int minx = min(min(pay[0].pos[0], pay[1].pos[0]), min(pay[1].pos[0], pay[2].pos[0]));
    int miny = min(min(pay[0].pos[1], pay[1].pos[1]), min(pay[1].pos[1], pay[2].pos[1]));
    int maxx = max(max(pay[0].pos[0], pay[1].pos[0]), max(pay[1].pos[0], pay[2].pos[0]));
    int maxy = max(max(pay[0].pos[1], pay[1].pos[1]), max(pay[1].pos[1], pay[2].pos[1]));
    maxx = maxx < (getWidth() - 1) ? maxx : getWidth() - 1;
    maxy = maxy < (getHeight() - 1) ? maxy : getHeight() - 1;
    minx = minx > 0 ? minx : 0;
    miny = miny > 0 ? miny : 0;


    return {minx, maxx, miny, maxy};
}

tuple<float, float, float> getBarycentric2D(vec3 A, vec3 B, vec3 C, vec3 P) {
    vec3 s[2];
    for (int i = 2; i--;) {
        s[i][0] = C[i] - A[i];
        s[i][1] = B[i] - A[i];
        s[i][2] = A[i] - P[i];
    }
    vec3 u = cross(s[0], s[1]);
    if (abs(u.z) > 1e-2) {
        u = u / u.z;
        return tuple<float, float, float>(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
    }
    return tuple<float, float, float>(-1, -1, -1);
}

vec3 interpolate(float alpha, float beta, float gamma, vec3 A, vec3 B, vec3 C) {
    return alpha * A + beta * B + gamma * C;
}
