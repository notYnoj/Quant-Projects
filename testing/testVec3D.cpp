#include "../Math Algorithms/3DVector.hpp"
#include <iostream>

int main(){
    Vec3D a(1,2,3);
    a.print();
    Vec3D B(1,2,3);
    std::cout<< a * (B);

}