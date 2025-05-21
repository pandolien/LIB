#include "pch.h"
#include "Robot/lParam.h"
#include "3d/dhMat.h"
pJoint6::pJoint6(){
    Initialization();
}
pJoint6::~pJoint6(){
    Close();
}
void pJoint6::Initialization(){
    q1 = 0;
    q2 = 0;
    q3 = 0;
    q4 = 0;
    q5 = 0;
    q6 = 0;
}
void pJoint6::Close(){
    q1 = 0;
    q2 = 0;
    q3 = 0;
    q4 = 0;
    q5 = 0;
    q6 = 0;
}
pJoint6 pJoint6::RAD(){
    pJoint6 out;
    out.q1 = RAD(q1);
    out.q2 = RAD(q2);
    out.q3 = RAD(q3);
    out.q4 = RAD(q4);
    out.q5 = RAD(q5);
    out.q6 = RAD(q6);
    return out;
}

pJoint6 pJoint6::DEG(){
    pJoint6 out;
    out.q1 = DEG(q1);
    out.q2 = DEG(q2);
    out.q3 = DEG(q3);
    out.q4 = DEG(q4);
    out.q5 = DEG(q5);
    out.q6 = DEG(q6);
    return out;
}