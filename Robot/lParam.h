#ifndef __LPARAM_H__
#define __LPARAM_H__
class pJoint6{
public:
    pJoint6();
    pJoint6(float,float,float,float,float,float);
    ~pJoint6();
public:
    void Initialization();
    void Close();
public:
    float q1,q2,q3,q4,q5,q6;
    pJoint6 RAD();
    pJoint6 DEG();
};
class pAxis{
public:
    pAxis();
    pAxis(float,float,float,float,float,float);
    ~pAxis();
public:
    void Initialization();
    void Close();
public:
    float x,y,z,a,b,r;
};
#endif