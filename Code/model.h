//
// Created by 42450 on 2023/9/10.
//
#ifndef MYRENDERER_MODEL_H
#define MYRENDERER_MODEL_H

#include<stdlib.h>
#include"ThirdParties/geometry.h"
#include <vector>
#include"ThirdParties/tgaimage.h"

struct faceInfo{
    std::vector<vec3> verts;
    std::vector<vec3> texs;
    std::vector<vec3> norms;
    std::vector<vec4> hVerts;
};
class Model {
private:
    std::vector<vec3> verts;
    std::vector<vec3> uvs;
    std::vector<vec3> normals;
    std::vector<faceInfo> faceInfos;
    TGAImage diffuseTexture;
    TGAImage normalTexture;
    bool ifPerspectived=false;

public:
    Model(const char *filename);

    ~Model();

    int nFaces();

    vec3 getVert(int i);

    vec3 getUV(int i);

    vec3 getNorm(int i);

    vec3 getTextureNorm(float u,float v);

    faceInfo getFace(int i);

    void loadDiffuseTexture(char *filename);

    void loadNormalTexture(char *filename);

    TGAColor getTextureColor(float u,float v);

    void perspective(float camPos);

    std::vector<faceInfo>* getAllFaces();

};

#endif //MYRENDERER_MODEL_H
