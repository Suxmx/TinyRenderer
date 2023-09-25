//
// Created by 42450 on 2023/9/16.
//

#include "rasterizer.h"


void rasterizer::setModelMat(mat<4, 4> &_modelMat) {
    this->modelMat = _modelMat;
}

void rasterizer::setViewMat(mat<4, 4> &_viewMat) {
    this->viewMat = _viewMat;
}

void rasterizer::setProjMat(mat<4, 4> &_projMat) {
    this->projMat = _projMat;
}

void rasterizer::setVertexShader(std::function<vec3(vertexShaderPayload &)> &vertexShader) {
    this->vertexShader = vertexShader;
}

void rasterizer::setFragShader(function<TGAColor(fragShaderPayload &)> &fragShader) {
    this->fragShader = fragShader;
}


void rasterizer::draw(const vector<faceInfo> &faces) {
    drawNums++;
    Buffer<float> zBuffer(width, height);
    zBuffer.setAll(-255);
    Buffer<TGAColor> colorBuffer(width, height);
    for (auto face: faces) {
        vector<vertexShaderPayload> vps;
        vector<vec3> ndcPos;
        for (int t = 0; t < 3; t++) {
            vertexShaderPayload vp(face.verts[t], modelMat, viewMat, projMat);
            vertexShader(vp);
            ndcPos.push_back(vp.pos);
            vp.pos.x = (vp.pos.x + 1.) / 2 * width;
            vp.pos.y = (vp.pos.y + 1.) / 2 * height;
            vps.push_back(vp);
        }
        shader->vertexs = ndcPos;
        auto [minx, maxx, miny, maxy] = getBoundingBox(vps);
        for (int y = miny; y <= maxy; y++) {
            for (int x = minx; x <= maxx; x++) {
                auto [alpha, beta, gamma] = getBarycentric2D(vps[0].pos, vps[1].pos, vps[2].pos, vec3(x, y, 1));
                if (alpha < 0 || beta < 0 || gamma < 0)continue;
                auto interpolateNormal = interpolate(alpha, beta, gamma, face.norms[0], face.norms[1], face.norms[2]);
                auto interpolateUV = interpolate(alpha, beta, gamma, face.texs[0], face.texs[1], face.texs[2]);
                float interpolateZ = alpha * vps[0].pos.z + beta * vps[1].pos.z + gamma * vps[2].pos.z;
                fragShaderPayload fp(interpolateNormal, interpolateUV, &*texture);
                if (normTexture)fp.normalTexture = &*normTexture;
                TGAColor color = fragShader(fp);
                if (interpolateZ < zBuffer.getValue(x, y))continue;
                zBuffer.setValue(x, y, interpolateZ);
                colorBuffer.setValue(x, y, color);
            }
        }

    }
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            TGAColor color = colorBuffer.getValue(x, y);
            image.set(x, y, colorBuffer.getValue(x, y));
        }
    }
    image.flip_vertically();
    string name = "output_";
    name.append(to_string(drawNums)).append(".tga");

    image.write_tga_file(name.c_str());
}

rasterizer::rasterizer() {

}

void rasterizer::setTexture(Texture &texture) {
    this->texture = texture;
}

void rasterizer::clear() {

}

void rasterizer::setShader(Shader *shader) {
    std::function<vec3(vertexShaderPayload &payload)> vertexShader = [shader](vertexShaderPayload &payload) {
        return shader->vertexShader(payload);
    };
    std::function<TGAColor(fragShaderPayload &)> fragShader = [shader](fragShaderPayload &payload) {
        return shader->fragShader(payload);
    };
    setVertexShader(vertexShader);
    setFragShader(fragShader);
    this->shader = shader;
}

void rasterizer::setNormalTexture(NormalTexture &norm) {
    this->normTexture = norm;
}


