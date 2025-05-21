#ifndef __DHVECTOR_H__
#define __DHVECTOR_H__
class dhVector{
public:
    dhVector();
    dhVector(float x,float y);
    dhVector(float x,float y,float z);
public:
    float x,y,z;
public:
    dhVector operator +(dhVector&);
    dhVector operator -(dhVector&);
    dhVector operator *(dhVector&);
    dhVector operator *(float);
    dhVector operator /(float);
    
public:
    float Norm();
    dhVector Unit();
    float Norm(dhVector&);
    dhVector Unit(dhVector&);
    dhVector Rand();
    float Dot(dhVector&);
};
#endif
