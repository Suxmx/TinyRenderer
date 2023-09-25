#include "renderer.h"
#include "utilities.h"
#include <iostream>
#include <algorithm>

using namespace std;
int map[501][501] = {0};

//线条绘制
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    int ierror = 0;
    if (abs(x0 - x1) < abs(y0 - y1)) {
        if (y1 < y0) {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        int x = x0;
        int dx2 = 2 * abs(x1 - x0), dy = abs(y1 - y0);
        for (int i = y0; i <= y1; i++) {
            image.set(x, i, color);
            ierror += dx2;
            if (ierror > dy) {
                x += (x1 > x0) ? 1 : -1;
                ierror -= 2 * dy;
//                cout<<x<<" "<<i<<endl;
            }
        }
    } else {
        if (x1 < x0) {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        int y = y0;
        int dx = abs(x1 - x0), dy2 = 2 * abs(y1 - y0);
        for (int i = x0; i <= x1; i++) {
            image.set(i, y, color);
            ierror += dy2;
            if (ierror > dx) {
                y += (y1 > y0) ? 1 : -1;
                ierror -= 2 * dx;
//                cout<<i<<" "<<y<<endl;
            }
        }
    }
}

//实现zbuffer的单色三角形绘制
void triangle(vector<vec3> points, vector<vector<float>> &zbuffer, TGAImage &image, TGAColor color) {

    int minx = min(min(points[0][0], points[1][0]), min(points[1][0], points[2][0]));
    int miny = min(min(points[0][1], points[1][1]), min(points[1][1], points[2][1]));
    int maxx = max(max(points[0][0], points[1][0]), max(points[1][0], points[2][0]));
    int maxy = max(max(points[0][1], points[1][1]), max(points[1][1], points[2][1]));
    maxx = maxx < (getWidth() - 1) ? maxx : getWidth() - 1;
    maxy = maxy < (getHeight() - 1) ? maxy : getHeight() - 1;
    minx = minx > 0 ? minx : 0;
    miny = miny > 0 ? miny : 0;

    for (int i = miny; i <= maxy; i++)
        for (int j = minx; j <= maxx; j++) {
            vec3 p(j, i, 0);
            vec3 uv = getBarycentric(points[0], points[1], points[2], p);
            if (uv.x < 0 || uv.y < 0 || uv.z < 0)continue;
            for (int m = 0; m < 3; m++)p.z += uv[m] * points[m][2];
            if (p.z > zbuffer[j][i]) {
                zbuffer[1][1] = 1;
                zbuffer[j][i] = p.z;
                image.set(j, i, color);
                map[j][i] += 1;

            }
        }
}

//绘制模型框架
void drawMesh(Model &model, int width, int height, TGAImage &image, TGAColor color) {
    for (int i = 0; i < model.nFaces(); i++) {
        vector<vec3> verts = model.getFace(i).verts;
        line((verts[0][0] + 1.) * width / 2, (verts[0][1] + 1.) * height / 2, (verts[1][0] + 1.) * width / 2,
             (verts[1][1] + 1.) * height / 2,
             image, color);
        line((verts[0][0] + 1.) * width / 2, (verts[0][1] + 1.) * height / 2, (verts[2][0] + 1.) * width / 2,
             (verts[2][1] + 1.) * height / 2,
             image, color);
        line((verts[2][0] + 1.) * width / 2, (verts[2][1] + 1.) * height / 2, (verts[1][0] + 1.) * width / 2,
             (verts[1][1] + 1.) * height / 2,
             image, color);

    }
}

//平面着色
void flatShading(Model &model, int width, int height, vector<vector<float>> &zbuffer, TGAImage &image) {
    vec3 lightDir(0, 0, -1);
    for (int i = 0; i < model.nFaces(); i++) {
        vector<vec3> worldCoords;
        worldCoords = model.getFace(i).verts;
        vector<vec3> screenCoords;
        for (int j = 0; j < 3; j++) {
            vec3 v((worldCoords[j][0] + 1.) / 2 * width, (worldCoords[j][1] + 1.) / 2 * height, worldCoords[j][2]);
            screenCoords.push_back(v);
        }
        vec3 n = cross( (worldCoords[2] - worldCoords[0]),(worldCoords[1] - worldCoords[0]));
        n = n.normalized();

        float intensity = n * lightDir;

        if (intensity > 0) {
            int tmp = 255 * intensity;
            TGAColor color(tmp, tmp, tmp, 255);
            triangle(screenCoords, zbuffer, image, color);
        }
    }
}

//纹理映射
void
diffuseShading(Model &model, char *texture, int width, int height, vector<vector<float>> &zbuffer, TGAImage &image) {
    vec3 lightDir(0, 0, -1);
    model.loadDiffuseTexture(texture);
    for (int i = 0; i < model.nFaces(); i++) {
        vector<vec3> worldCoords;
        worldCoords = model.getFace(i).verts;
        vector<vec3> screenCoords;
        vector<vec3> texUV = model.getFace(i).texs;
        for (int j = 0; j < 3; j++) {
            vec3 v((worldCoords[j][0] + 1.) / 2 * width, (worldCoords[j][1] + 1.) / 2 * height, worldCoords[j][2]);
            screenCoords.push_back(v);
        }
        int minx = min(min(screenCoords[0][0], screenCoords[1][0]), min(screenCoords[1][0], screenCoords[2][0]));
        int miny = min(min(screenCoords[0][1], screenCoords[1][1]), min(screenCoords[1][1], screenCoords[2][1]));
        int maxx = max(max(screenCoords[0][0], screenCoords[1][0]), max(screenCoords[1][0], screenCoords[2][0]));
        int maxy = max(max(screenCoords[0][1], screenCoords[1][1]), max(screenCoords[1][1], screenCoords[2][1]));
        maxx = maxx < (width - 1) ? maxx : width - 1;
        maxy = maxy < (height - 1) ? maxy : height - 1;
        minx = minx > 0 ? minx : 0;
        miny = miny > 0 ? miny : 0;

//        std::cout << minx << ' ' << maxx << ' ' << miny << ' ' << maxy << '\n';
        for (int y = miny; y <= maxy; y++) {
            for (int x = minx; x <= maxx; x++) {
                vec3 p(x, y, 0);
                //获取重心坐标
                vec3 uv = getBarycentric(screenCoords[0], screenCoords[1], screenCoords[2], p);
                if (uv.x < 0 || uv.y < 0 || uv.z < 0)continue;
                //利用重心坐标插值得到z
                for (int m = 0; m < 3; m++)p.z += uv[m] * screenCoords[m][2];
                if (p.z < zbuffer[x][y])continue;
                //获取面法线
                vec3 n = cross( (worldCoords[2] - worldCoords[0]),(worldCoords[1] - worldCoords[0]));
                n = n.normalized();
                //根据法线求得光照强度
                float intensity = n * lightDir;
                if (intensity < 0)continue;
                zbuffer[x][y] = p.z;
                vec3 uvP;
                for (int m = 0; m < 3; m++)
                    uvP = uvP + texUV[m] * uv[m];
                TGAColor color = model.getTextureColor(uvP.x, uvP.y);
                color = color * intensity;
                color.a = static_cast<unsigned char>(255);
                image.set(x, y, color);
            }
        }

    }
}





