//
// Created by 42450 on 2023/9/18.
//

#include "Camera.h"
#include <iostream>
void Camera::setPos(vec3 pos) {
    this->pos = pos;
}

void Camera::setLookAt(vec3 lookAt) {
    this->lookAt = lookAt;
}

 mat<4, 4> Camera::getViewMat() {
    mat<4, 4> transform = mat<4, 4>::identity(), rotation;
    for (int i = 0; i < 3; i++) {
        transform[i][3] = -1. * pos[i];
    }
    rotation[3][3]=1;
     std::cout<<x<<"\n"<<y<<"\n"<<lookAt<<"\n";
    for(int i=0;i<3;i++){
        rotation[0][i]=x[i];
        rotation[1][i]=y[i];
        rotation[2][i]=lookAt[i];
    }std::cout<<rotation;
    rotation=rotation.transpose();

    mat<4,4> tmp=rotation*transform;
    return tmp;

}

void Camera::setWorldUp(vec3 worldUp) {
    this->worldUp = worldUp;
}

Camera::Camera(vec3 pos, vec3 lookAt) {
    this->lookAt=lookAt.normalized();
    this->pos=pos;
    x= cross(worldUp,lookAt);
    y=cross(lookAt,x);
    x=x.normalized();y=y.normalized();

}
