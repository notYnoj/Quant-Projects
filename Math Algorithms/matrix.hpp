#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include <vector>
#include <utility>
#include <cmath>
#include "3DVector.hpp"

struct Matrix {
public:
    size_t rows, columns;
    std::vector<double> data;

    Matrix(const size_t& _rows, const size_t& _columns);
    
    void append(size_t row, size_t col, double value);
    double get(size_t row, size_t col);
    double get(size_t row, size_t col) const;
    double& operator()(size_t row, size_t col);
    const double& operator()(size_t row, size_t col) const;
    
    Matrix operator+(const Matrix& Addend) const;
    Matrix& operator+=(const Matrix& Addend);
    Matrix operator*(const double& scalar) const;
    friend Matrix operator*(double scalar, const Matrix& mat);
    Matrix& operator*=(const double& scalar);
    Matrix operator-(const Matrix& Subtrahend) const;
    Matrix& operator-=(const Matrix& Subtrahend);
    Matrix operator*(const Matrix& Factor) const;
    Matrix& operator*=(const Matrix& Factor);
    
    Matrix T() const;
    std::pair<Matrix, Matrix> LU_Decomposition() const;
    double det() const;
    Matrix minor(size_t remove_row, size_t remove_col) const;
    Matrix cof() const;
    Matrix adj() const;
    Matrix inv() const;

    void print() const;
    static Matrix lookAt(const Vec3D& eye, const Vec3D& focus, const Vec3D& up);
};

//friend function stuff
Matrix operator*(double scalar, const Matrix& mat);
#endif