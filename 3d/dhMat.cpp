#include "pch.h"
#include "dhMat.h"
dhMat::dhMat()
{
    int i,j,cnt = 0;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (i == j) v[cnt++] = 1;
            else v[cnt++] = 0;
        }
    }
}


dhMat::~dhMat()
{

}




dhMat dhMat::operator*(dhMat h)
{
    dhMat Out;
    int i, j, k, cnt = 0;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            float val = 0;
            for (k = 0; k < 4; k++) {val += v[k * 4 + j] * h.v[i * 4 + k];}
            Out.v[cnt++] = val;
        }
    }
    return Out;
}

dhVector dhMat::operator*(dhVector l)
{
    dhVector V;
    V.x = l.x * v[0] + l.y * v[4] + l.z * v[8] + 1 * v[12];
    V.y = l.x * v[1] + l.y * v[5] + l.z * v[9] + 1 * v[13];
    V.z = l.x * v[2] + l.y * v[6] + l.z * v[10] + 1 * v[14];
    return V;
}

dhMat dhMat::Trans(float x, float y,float z)
{
    dhMat h;
    h.v[12] = x;
    h.v[13] = y;
    h.v[14] = z;
    return h;
}
//angle is Radian
dhMat dhMat::RotX(float angle)
{
    dhMat h;
    float s, c;
    s = sin(angle);
    c = cos(angle);
    h.v[5] = c;
    h.v[9] = -s;
    h.v[6] = s;
    h.v[10] = c;
    return h;
}

dhMat dhMat::RotY(float angle)
{
    dhMat h;
    float s, c;
    s = sin(angle);
    c = cos(angle);
    h.v[0] = c;
    h.v[8] = s;
    h.v[2] = -s;
    h.v[10] = c;
    return h;
}


dhMat dhMat::RotZ(float angle)
{
    dhMat h;
    float s, c;
    s = sin(angle);
    c = cos(angle);
    h.v[0] = c;
    h.v[4] = -s;
    h.v[1] = s;
    h.v[5] = c;
    return h;
}

dhMat dhMat::Scale(float s)
{
    dhMat h;
    h.v[0] = s;
    h.v[5] = s;
    return h;
}

dhMat dhMat::Scale(float sx, float sy)
{
    dhMat h;
    h.v[0] = sx;
    h.v[5] = sy;
    return h;
}

dhMat dhMat::Scale(float sx, float sy, float sz)
{
    dhMat h;
    h.v[0] = sx;
    h.v[5] = sy;
    h.v[10] = sz;
    return h;
}

dhMat dhMat::Scale(dhVector v)
{
    dhMat h;
    h.v[0] = v.x;
    h.v[5] = v.y;
    h.v[10] = v.z;
    return h;
}

dhMat dhMat::Inv()
{
    dhMat ret;
    ret.v[0] = v[0];
    ret.v[1] = v[4];
    ret.v[2] = v[8];

    ret.v[4] = v[1];
    ret.v[5] = v[5];
    ret.v[6] = v[9];

    ret.v[8] = v[2];
    ret.v[9] = v[6];
    ret.v[10] = v[10];
    dhVector Vt = O()*-1.0;
    dhVector Vt2 = ret * Vt;
    ret.v[12] = Vt2.x;
    ret.v[13] = Vt2.y;
    ret.v[14] = Vt2.z;
    return ret;
}

dhMat dhMat::Quat(float q, dhVector v)
{
    dhMat h;
    v = v.Unit();
    float halfq = q * 2;
    float w = cos(halfq);
    float hs = sin(halfq);
    dhVector v2 = v * hs;
    v = v.Unit();
    
    h.v[0] =    1 - 2 * (v2.y * v2.y + v2.z * v2.z);
    h.v[1] =    2 * (v2.x*v2.y+w*v2.z);
    h.v[2] =   2 * (v2.x * v2.z - w * v2.y);

    h.v[4] =   2 * (v2.x *v2.y - w*v2.z);
    h.v[5] =    1 - 2 * (v2.x * v2.x + v2.z * v2.z);
    h.v[6] =   2 * (v2.y * v2.z + w * v2.x);

    h.v[8] =   2 * (v2.x * v2.z + w * v2.y);
    h.v[9] =   2 * (v2.y * v2.z - w * v2.x);
    h.v[10] =  1 - 2 * (v2.x * v2.x + v2.y * v2.y);
    return h;
}
dhMat dhMat::Quat(dhQuat& q){
    dhMat h;
    float x2, y2, z2;
    float xy, xz, xw;
    float yz, yw;
    float zw;

    x2 = q.x*q.x;
    y2 = q.y*q.y;
    z2 = q.z*q.z;
    xy = q.x*q.y;
    xz = q.x*q.z;
    xw = q.x*q.w;
    yz = q.y*q.z;
    yw = q.y*q.w;
    zw = q.z*q.w;
    h.v[0] = 1-2*(y2+z2);
    h.v[5] = 1-2*(x2+z2);
    h.v[10] = 1-2*(x2+y2);

    h.v[1] = 2*xy -2 * zw;
    h.v[2] = 2*xz + 2*yw;
    h.v[4] = 2*xy + 2*zw;

    h.v[6] = 2*yz - 2*xw;
    h.v[8] = 2*xz - 2*yw;
    h.v[9] = 2*yz+2*xw;
    return h;
}
dhQuat dhMat::GetQuat(){
    dhQuat q;
    float S,t,big;
    t = v[0]+v[5]+v[10];
    if(t >0){
        S = 2*sqrt(t+1);
        q.x = (v[6]-v[9])/S;
        q.y = (v[8]-v[3])/S;
        q.z = (v[1]-v[4])/S;
        q.w = S/4;
    }
    else{
        big = MAX(v[0],v[5]);
        big = MAX(v[10],big);
        if(big == v[0]){
            S = 2*sqrt(1+v[0]-v[5]-v[10]);
            q.x = S*0.25;
            q.w = (v[6]-v[9])/S;
            q.y = (v[1]-v[4])/S;
            q.z = (v[8]-v[3])/S;
        }
        if(big == v[5]){
            S = 2*sqrt(1+v[5]-v[0]-v[10]);
            q.y = S*0.25;
            q.w = (v[8]-v[3])/S;
            q.x = (v[1]+v[4])/S;
            q.z = (v[6]+v[9])/S;
        }
        if(big == v[10]){
            S = 2*sqrt(1+v[10]-v[0]-v[5]);
            q.w = (v[1]-v[4])/S;
            q.x = (v[8]+v[3])/S;
            q.y = (v[6]+v[9])/S;
            q.z = S*0.25;
        }
    }
    return q;
}
_RPY dhMat::RPY()
{
    _RPY abr;
    float cr31;
    abr.b = atan2(-v[2], sqrt(v[0] * v[0] + v[1] * v[1]));
    cr31 = cos(abr.b);
    abr.a = atan2(v[1] / cr31, v[0] / cr31);
    abr.r = atan2(v[6] / cr31, v[10] / cr31);
    return abr;
}
void dhMat::Hmatrix(float x, float y, float z, float a, float b, float r)
{
    dhMat h;
    h	 = h.Trans(x,y,z)*h.RotZ(a)*h.RotY(b)*h.RotX(r);
    memmove(v, h.v, sizeof(float) * 16);
}

dhVector dhMat::O()
{
    dhVector h;
    h.x = v[12];
    h.y = v[13];
    h.z = v[14];
    return h;
}


