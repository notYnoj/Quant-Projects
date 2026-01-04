#include "../cameras/firstPerson.hpp"

FirstPersonCamera::FirstPersonCamera(Vec3D _position, Yaw _yaw, Pitch _pitch, Roll _roll, Speed _speed, Sens _sens) : Position(_position), yaw(_yaw), pitch(_pitch), roll(_roll), speed(_speed), sens(_sens) {
    updateCamera();
}
double FirstPersonCamera::toRad(double degrees){
    return (degrees * (M_PI/180.0));
}
void FirstPersonCamera::updateCamera(){
    normalUp = Vec3D(0,1,0);
    if(pitch.x > 89.0){
        pitch.x = 89.0;
    }
    if(pitch.x < -89.0){
        pitch.x = -89.0;
    }

    if(yaw.x > 360.0){
        yaw.x -= 360.0;
    }
    if(yaw.x < 360.0){
        yaw.x += 360.0;
    }

    front.x() = cos(toRad(yaw.x)) * sin(toRad(pitch.x));
    front.y() = -cos(toRad(pitch.x));
    front.z() = sin(toRad(yaw.x)) * sin(toRad(pitch.x)); 

    front = front.normal();

    right = front.cross(normalUp);

    if(right.magnitude()<0.001){
        right = Vec3D(1,0,0);
    }else{
        right = right.normal();
    }
    up = right.cross(front);
    if(up.magnitude() < 0.001){
        up = normalUp;
    }else{
        up = up.normal();
    }
        //front = whatever we find -> normalUp cross this is right then front cross right is = up
}

void FirstPersonCamera::processWSADInput(GLFWwindow* window, double time){
    double velocity = speed.x * time;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Position = Position + (front * velocity);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Position = Position - (front * velocity);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Position = Position - (right * velocity);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Position = Position + (right * velocity);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        Position = Position + (normalUp * velocity);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        Position = Position - (normalUp * velocity);
}
