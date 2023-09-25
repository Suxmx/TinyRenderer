#include<fstream>
#include <string>
#include <sstream>
#include "model.h"

using namespace std;

Model::Model(const char *filename) {
    ifstream filestream;
    filestream.open(filename, ifstream::in);
    if (filestream.fail())
        return;
    string line;
    char trash;
    int intTrash;
    while (!filestream.eof()) {
        getline(filestream, line);
        istringstream iss(line.c_str());
        if (!line.compare(0, 2, "v ")) {
            vec3 vert;
            iss >> trash;
            for (int i = 0; i < 3; i++)
                iss >> vert[i];
            verts.push_back(vert);
        } else if (!line.compare(0, 3, "vt ")) {
            vec3 uv;
            iss >> trash >> trash;
            for (int i = 0; i < 3; i++)
                iss >> uv[i];
            uvs.push_back(uv);
        } else if (!line.compare(0, 3, "vn ")) {
            vec3 norm;
            iss >> trash >> trash;
            for (int i = 0; i < 3; i++)
                iss >> norm[i];
            normals.push_back(norm);
        } else if (!line.compare(0, 2, "f ")) {
            int nVert, nUV, nNorm;
            faceInfo face;
            vector<int> tnum;
            iss >> trash;
            for (int i = 0; i < 3; i++) {
                iss >> nVert >> trash >> nUV >> trash >> nNorm;
                face.verts.push_back(getVert(nVert - 1));
                face.texs.push_back(getUV(nUV - 1));
                face.norms.push_back(getNorm(nNorm - 1));
                vec3 tmp= getVert(nVert-1);
                vec4 hv;
                hv[3]=1;
                for(int m=0;m<3;m++)hv[m]=tmp[m];
                face.hVerts.push_back(hv);
                tnum.push_back(nVert);
            }
            faceInfos.push_back(face);
        }

    }
//    cout<<"VertexNumber:"<<verts.size();
}

Model::~Model() {
}

vec3 Model::getVert(int i) {
    return verts[i];
}

void Model::loadDiffuseTexture(char *filename) {
    diffuseTexture.read_tga_file(filename);
    diffuseTexture.flip_vertically();
}



vec3 Model::getUV(int i) {
    return uvs[i];
}

faceInfo Model::getFace(int i) {
    return faceInfos[i];
}

vec3 Model::getNorm(int i) {
    return normals[i];
}

int Model::nFaces() {
    return faceInfos.size();
}

TGAColor Model::getTextureColor(float u, float v) {
    int width = diffuseTexture.get_width();
    int height = diffuseTexture.get_height();
    return diffuseTexture.get(u * width, v * height);
}

void Model::perspective(float camPos) {
    if(ifPerspectived)return;
    ifPerspectived=true;
    mat<4, 4> pMat = mat<4, 4>::identity();
    pMat[3][2] = -1. / camPos;
    faceInfo *face;
    mat<4, 1> hCoord;
    for (int i = 0; i < nFaces(); i++) {
        face = &faceInfos[i];
        for (int m = 0; m < 3; m++) {
            vec3 v = face->verts[m];
            for (int y = 0; y < 3; y++)hCoord[y][0] = v[y];
            hCoord[3][0] = 1;
            hCoord = pMat * hCoord;
            vec3 temp;
            for (int y = 0; y < 3; y++) {
                //(face->verts)[m][y] = hCoord[y][0] / hCoord[3][0];
                temp[y] = hCoord[y][0] / hCoord[3][0];
            }
            faceInfos[i].verts[m] = temp;
        }
        face = nullptr;
    }
}

std::vector<faceInfo> *Model::getAllFaces() {
    return &faceInfos;
}

void Model::loadNormalTexture(char *filename) {
    normalTexture.read_tga_file(filename);
    normalTexture.flip_vertically();
}

vec3 Model::getTextureNorm(float u, float v) {
    int width = normalTexture.get_width();
    int height = normalTexture.get_height();
    TGAColor color= normalTexture.get(u * width, v * height);
    vec3 norm;
    norm.x=(static_cast<int>(color.r)/255.0)*2-1;
    norm.y=(static_cast<int>(color.g)/255.0)*2-1;
    norm.z=(static_cast<int>(color.b)/255.0)*2-1;
    return norm;
}


