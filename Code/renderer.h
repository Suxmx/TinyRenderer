#ifndef MYRENDERER_RENDERER_H
#define MYRENDERER_RENDERER_H

#include "ThirdParties/tgaimage.h"
#include "ThirdParties/geometry.h"
#include "model.h"
#include <vector>

using namespace std;

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);

void triangle(vector<vec3>, vector<vector<float>> &zbuffer, TGAImage &image, TGAColor color);

void drawMesh(Model &model, int width, int height, TGAImage &image, TGAColor color);

void flatShading(Model &model, int width, int height, vector<vector<float>> &zbuffer, TGAImage &image);

void
diffuseShading(Model &model, char *texture, int width, int height, vector<vector<float>> &zbuffer, TGAImage &image);

void
pDiffuseShading(Model &model, char *texture,float camPos, int width, int height, vector<vector<float>> &zbuffer, TGAImage &image);
#endif //MYRENDERER_RENDERER_H
