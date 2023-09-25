//
// Created by 42450 on 2023/9/18.
//

#ifndef MYRENDERER_CAMERA_H
#define MYRENDERER_CAMERA_H
#include "ThirdParties/geometry.h"

class Camera {
private:
    vec3 pos=vec3(0,0,0);
    vec3 lookAt;//z
    vec3 y;
    vec3 x;
    vec3 worldUp=vec3(0,1,0);
public:
    Camera(vec3 pos,vec3 lookAt);
    void setPos(vec3 pos);
    void setLookAt(vec3 lookAt);
    void setWorldUp(vec3 worldUp);
     mat<4,4> getViewMat();
};


#endif //MYRENDERER_CAMERA_H
