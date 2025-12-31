#include <iostream>
#include <vector>
#include <stdexcept>
#include <utility>
#include <float.h>
#include <cmath>

struct Vec3D{
    long long x,y,z;
    std::array<long long, 3> vec;

    Vec3D(const long long& _x, const long long& _y, const long long& _z) : x(_x), y(_y), z(_z) {}
    /*
    Intended Functions:
    Add Done
    Subtract
    Scalar Multiplication
    Dot Product
    Cross Product
    Magnitutde
    Normalize
    Projection
    Reflection
    Angle Between Vectors

    */

    Vec3D operator+(const Vec3D& Addend) const{
        Vec3D temp(0,0,0);
        temp.vec[0] = vec[0] + Addend.vec[0];
        temp.vec[1] = vec[1] + Addend.vec[1];
        temp.vec[2] = vec[2] + Addend.vec[2];
        return temp;
    }

    Vec3D& operator+=(const Vec3D& Addend){
        Vec3D temp(0,0,0);
        vec[0] += Addend.vec[0];
        vec[1] += Addend.vec[1];
        vec[2] += Addend.vec[2];
        return *this;
    }
    Vec3D operator*(const double& Factor) const{
        Vec3D temp = *this;
        temp.vec[0] *= Factor;
        temp.vec[1] *= Factor;
        temp.vec[2] *= Factor;
        return temp;
    }
    
    Vec3D& operator*=(const double& Factor){
        vec[0] *= Factor;
        vec[1] *= Factor;
        vec[2] *= Factor;
        return *this;
    }
    



};