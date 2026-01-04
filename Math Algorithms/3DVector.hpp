#ifndef VEC3D_HPP
#define VEC3D_HPP

#include <iostream>
#include <vector>
#include <stdexcept>
#include <utility>
#include <array> 
#include <float.h>
#include <cmath>

struct Vec3D{
    std::array<double, 3> vec;

    Vec3D(const double& _x, const double& _y, const double& _z) : vec{_x, _y, _z} {}

    Vec3D() : vec{0.0, 0.0, 0.0} {}

    const double& x() const{ return vec[0]; }

    const double& y() const{ return vec[1]; }

    const double& z() const{ return vec[2]; }

    double& x() { return vec[0]; }

    double& y() { return vec[1]; }

    double& z() { return vec[2]; }

    /*
    Intended Functions:
    Add Done
    Subtract Done
    Scalar Multiplication Done
    Dot Product Done
    Cross Product Done
    Magnitutde Done=
    Normalize Done
    Projection Done
    Reflection Done
    Angle Between Vectors Done
    */
    Vec3D operator+(const Vec3D& Addend) const{
        Vec3D temp(0,0,0);
        temp.vec[0] = vec[0] + Addend.vec[0];
        temp.vec[1] = vec[1] + Addend.vec[1];
        temp.vec[2] = vec[2] + Addend.vec[2];
        return temp;
    }

    Vec3D& operator+=(const Vec3D& Addend){
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
    friend Vec3D operator*(const double& scalar, const Vec3D& vector){
        return vector * scalar;
    }

    
    Vec3D& operator*=(const double& Factor){
        vec[0] *= Factor;
        vec[1] *= Factor;
        vec[2] *= Factor;
        return *this;
    }

    Vec3D operator-(const Vec3D& Subtrahend) const{
        Vec3D temp = *this;
        temp.vec[0] -= Subtrahend.vec[0];
        temp.vec[1] -= Subtrahend.vec[1];
        temp.vec[2] -= Subtrahend.vec[2];
        return temp;
    }

    Vec3D& operator-=(const Vec3D& Subtrahend){
        vec[0] -= Subtrahend.vec[0];
        vec[1] -= Subtrahend.vec[1];
        vec[2] -= Subtrahend.vec[2];
        return *this;
    }

    double operator*(const Vec3D& factor) const{
        return ((vec[0] * factor.vec[0])  + (vec[1] * factor.vec[1]) + (vec[2] * factor.vec[2]));
    }

    double magnitude() const{
        return (sqrt((vec[0] * vec[0]) + (vec[1] * vec[1]) + (vec[2] * vec[2])));
    }
    //Returns a Vec3D the normal of the current vector
    Vec3D normal() const{
        Vec3D temp(vec[0], vec[1], vec[2]);
        double mag = magnitude();
        if (mag < 1e-10) {
            throw std::runtime_error("Cannot normalize zero vector (Division by 0)");
        }
        double scalar = 1.0/mag;
        temp *= scalar;
        return temp;
    }
    //Current a Vec3D is A. Returns A cross Factor Passed in
    Vec3D cross(const Vec3D& Factor) const{
        Vec3D ans;
        ans.vec[0] = vec[1] * Factor.vec[2] - vec[2] * Factor.vec[1]; 
        ans.vec[1] = vec[2] * Factor.vec[0] - vec[0] * Factor.vec[2];
        ans.vec[2] = vec[0] * Factor.vec[1] - vec[1] * Factor.vec[0];
        return ans;
    }

    Vec3D proj(const Vec3D& B) const{
        //A proj onto B = a dot b / |b|^2  * b
        double bSquared = B.magnitude() * B.magnitude();
        if(bSquared < 1e-10){
            throw std::runtime_error("Cannot proj onto zero vector (Division by 0)");
        }
        return ((*this * B) / (bSquared) * B);
    }
    //reflect current vector given a N normal vector perpendicular to a surface
    Vec3D reflect(const Vec3D& NormalVec) const{
        //R = V - 2* projn(V)
        return (*this -  (2 * proj(NormalVec)));
    }

    double angleBetween(const Vec3D& B) const{
        double mags = magnitude() * B.magnitude();
        if (mags < 1e-20) {
            throw std::runtime_error("Cannot compute angle with zero vector");
        }
        //idk abt clamping lowk lol
        return (acos((*this * B) / (mags)));
    }

    void print() const {
        std::cout<< "(" << vec[0] << ", " << vec[1] << ", " << vec[2] << ")" << std::endl;
    }

    bool operator==(const Vec3D& other) const {
        return x() == other.x() && y() == other.y() && z() == other.z();
    }
    
    bool operator!=(const Vec3D& other) const {
        return !(*this == other);
    }
};

#endif