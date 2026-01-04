#ifndef FIRSTPERSON_HPP
#define FIRSTPERSON_HPP
#include "../Math Algorithms/3DVector.hpp"
#include <GLFW/glfw3.h>

//using GLFW 1st POV Camera:

struct Yaw {double x;};
struct Pitch {double x;};
struct Roll {double x;};
struct Speed { double x;};
struct Sens { double x;};
struct FirstPersonCamera{
    Yaw yaw;
    Pitch pitch;
    Roll roll;
    Speed speed;
    Sens sens;
    Vec3D Position;
    Vec3D up;
    Vec3D right;
    Vec3D front;
    Vec3D normalUp; 

    FirstPersonCamera(Vec3D _position, Yaw _yaw, Pitch _pitch, Roll _roll, Speed _speed, Sens _sens);

    static double toRad(double degree);
    void updateCamera();
    void processWSADInput(GLFWwindow* window, double time);
};

#endif
