#include "pch.h"
#include "dhVector.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
dhVector::dhVector(){
    x = 0;
    y = 0;
    z = 0;
} 
dhVector::dhVector(float x,float y){
    this->x = x;
    this->y = y;
    z = 0;
}
dhVector::dhVector(float x,float y,float z){
    this->x = x;
    this->y = y;
    this->z = z;
}
dhVector dhVector::operator+(dhVector&v){
    return dhVector(x+v.x,y+v.y,z +v.z);
}
dhVector dhVector::operator-(dhVector&v){
    return dhVector(x-v.x,y-v.y,z -v.z);
}
dhVector dhVector::operator*(dhVector&v){
    float i,j,k;
    i = y*v.z-z*v.y;
    j = -(z*v.x-x*v.z);
    k = x*v.y-v.x*y;
    return dhVector(i,j,k);
}
dhVector dhVector::operator*(float other){
    return dhVector(x*other,y*other,z*other);
}
dhVector dhVector::operator/(float other){
    return dhVector(x/other,y/other,z/other);
}
#include<math.h>
float dhVector::Norm(){
    return sqrt(x*x+y*y+z*z);
}
dhVector dhVector::Unit(){
    float N = Norm();
    return dhVector(x/N,y/N,z/N);
}
float dhVector::Norm(dhVector&v){
    float X = x-v.x;
    float Y = y-v.y;
    float Z = z-v.z;
    return sqrt(X*X+Y*Y+Z*Z);
}
dhVector dhVector::Unit(dhVector&v){
    float N = Norm(v);
    float X = x-v.x;
    float Y = y-v.y;
    float Z = z-v.z;
    return dhVector(X/N,Y/N,Z/N);
}

dhVector dhVector::Rand()
{
    return dhVector();
}

float dhVector::Dot(dhVector& v)
{
    return x *v.x + y*v.y + z *v.z;
}
