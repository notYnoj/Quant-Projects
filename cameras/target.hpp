#ifndef TARGET_HPP
#define TARGET_HPP

#include "../Math Algorithms/3DVector.hpp"
#include <vector>
#include <cmath>

/*
A camera that always remains locked to a target
Used in vector Visualizer
*/

struct Radius { double x; };
struct Theta  { double x; };
struct Phi    { double x; };
struct Speed  { double x; };
struct Sens   { double x; };

struct TargetCamera {
    Vec3D target;
    Radius radius;
    Theta theta;
    Phi phi;
    Speed speed;
    Sens sens;
    Vec3D Position;
    Vec3D front;
    Vec3D up;
    Vec3D right;
    Vec3D normalUp{0,1,0};

    TargetCamera(Vec3D _target, Radius _radius, Theta _theta, Phi _phi, Speed _speed, Sens _sens);

    static double toRad(double degree);
    void updateCamera();
};

#endif
