#ifndef TARGET_HPP
#define TARGET_HPP

#include "../Math Algorithms/3DVector.hpp"
#include <vector>
#include <cmath>
/*
A camera that always remains locked to a target
Used in vector Visualizer
*/
struct Radius { double x;};
struct Theta { double x;};
struct Phi { double x;};
struct Speed { double x;};
struct Sens { double x;};

struct TargetCamera{
    Vec3D target;       
    Radius radius;        
    Theta theta;        
    Phi phi;        
    Vec3D Position;
    Vec3D front;
    Vec3D up;
    Vec3D right;
    Vec3D normalUp{0,1,0};
    Speed speed;
    Sens sens;

    TargetCamera(Vec3D _target, Radius _radius, Theta _theta, Phi _phi, Speed _speed, Sens _sens) : target(_target), radius(_radius), theta(_theta), phi(_phi), speed(_speed), sens(_sens) {updateCamera();}


    double toRad(double degree){
        return (degree * (M_PI / 180.0));
    }

    void updateCamera(){
        if(phi.x > 179.0) phi.x = 179.0;
        if(phi.x < 1.0) phi.x = 1.0;
        if(theta.x > 360.0) theta.x -= 360.0;
        if(theta.x < 0.0) theta.x += 360.0;

        double thetaRad = toRad(theta.x);
        double phiRad = toRad(phi.x);

        Position.x() = target.x() + radius.x * sin(phiRad) * cos(thetaRad);
        Position.y() = target.y() + radius.x * cos(phiRad);
        Position.z() = target.z() + radius.x * sin(phiRad) * sin(thetaRad);

        front = (target - Position).normal();

        right = front.cross(normalUp);
        if(right.magnitude() < 0.001){
            right = Vec3D(1,0,0);
        }else{
            right = right.normal();
        }

        up = right.cross(front);
        if(up.magnitude() < 0.001){
            up = Vec3D(0,1,0);
        }else{
            up = up.normal();
        }

    }
};

#endif