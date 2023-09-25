#include "geometry.h"
#include <iostream>
using namespace std;
vec3 cross(const vec3 &v1, const vec3 &v2) {
//    cout<<"Vec1:"<<v1<<"||Vec2:"<<v2<<endl<<"\t";
//    cout<<v1.y*v2.z - v1.z*v2.y<<" "<<v1.z*v2.x - v1.x*v2.z<<" "<<v1.x*v2.y - v1.y*v2.x<<endl<<endl;
    return vec<3>{v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x};
}

