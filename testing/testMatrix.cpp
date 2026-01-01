#include "../Math Algorithms/matrix.hpp"
#include <iostream>

int main() {
    Matrix mat(3, 3);
    mat.append(0, 0, 1.0);
    mat.append(0, 1, 2.0);
    mat.append(0, 2, 3.0);
    mat.append(1, 0, 4.0);
    mat.append(1, 1, 5.0);
    mat.append(1, 2, 6.0);
    mat.append(2, 0, 7.0);
    mat.append(2, 1, 8.0);
    mat.append(2, 2, 9.0);

    std::cout << "Original matrix:" << std::endl;
    mat.print();
    
    std::cout << "\nMatrix * 2:" << std::endl;
    Matrix doubled = mat * 2.0;
    doubled.print();
    
    std::cout << "\nTranspose:" << std::endl;
    Matrix transposed = mat.T();
    transposed.print();
    
    std::cout << "\nDeterminant: " << mat.det() << std::endl;
    
    Matrix invertible(2, 2);
    invertible.append(0, 0, 4);
    invertible.append(0, 1, 7);
    invertible.append(1, 0, 2);
    invertible.append(1, 1, 6);
    
    std::cout << "\nInvertible matrix:" << std::endl;
    invertible.print();
    
    std::cout << "Determinant: " << invertible.det() << std::endl;
    
    try {
        Matrix inverse = invertible.inv();
        std::cout << "Inverse:" << std::endl;
        inverse.print();
    } catch(const std::exception& e) {
        std::cout << "Cannot invert: " << e.what() << std::endl;
    }
    
    return 0;
}