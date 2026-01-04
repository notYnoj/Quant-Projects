#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include <vector>
#include <stdexcept>
#include <utility>
#include <float.h>
#include <cmath>
#include "3DVector.hpp"

struct Matrix{
public:
    size_t rows, columns;
    std::vector<double> data;
    //(rows, cols) <- arguments
    Matrix(const size_t& _rows, const size_t& _columns) : rows(_rows), columns(_columns) {
        data.resize(rows*columns);
    }
    /*
    append(row, col, value) 0 Indexed
    */
    void append(size_t row, size_t col, double value){
        if(row >= rows || col >= columns){
            throw std::out_of_range("You are trying to insert an element that doesn't exist");
        }
        data[row * columns + col] = value;
    }

    double get(size_t row, size_t col) const{
        return data[row * columns + col];
    }
    
    Matrix operator+(const Matrix& Addend) const{
        if(Addend.rows != rows || Addend.columns != columns){
            throw std::invalid_argument("Addend Matrix does not have the same dimensions");
        }
        Matrix temp(rows, columns);
        for(size_t index{}; index < rows * columns; index++){
            temp.data[index] = data[index] + Addend.data[index];
        }
        return temp;
    }

    Matrix& operator+=(const Matrix& Addend){
        if(Addend.rows != rows || Addend.columns != columns){
            throw std::invalid_argument("Addend Matrix does not have the same dimensions");
        }
        for(size_t index{}; index < rows * columns; index++){
            data[index] = data[index] + Addend.data[index];
        }
        return *this;
    }
    
    Matrix operator*(const double& scalar) const {
        Matrix temp(rows, columns);
        for(size_t index{}; index < rows * columns; index++){
            if(data[index] > (DBL_MAX / scalar)){
                throw std::overflow_error("Overflow Error when attempting scalar multiplication. Index: " + std::to_string(index) + " Scalar: " + std::to_string(scalar));
            }
            temp.data[index] = data[index] * scalar;
        }
        return temp;
    }

    friend Matrix operator*(double scalar, const Matrix& mat) {
        return mat * scalar;
    }

    Matrix& operator*=(const double& scalar){
        for(size_t index{}; index < rows * columns; index++){
            if(data[index]>DBL_MAX / scalar){
                throw std::overflow_error("Overflow Error when attempting scalar multiplication. Index: " + std::to_string(index) + " Scalar: " + std::to_string(scalar));
            }
            data[index] *= scalar;
        }
        return *this;
    }

    Matrix operator-(const Matrix& Subtrahend) const{
        if(Subtrahend.rows != rows || Subtrahend.columns != columns){
            throw std::invalid_argument("Subtrahend Matrix does not have the same dimensions");
        }
        return (*this + (Subtrahend * -1.0));
    }

    Matrix& operator -=(const Matrix& Subtrahend){
        if(Subtrahend.rows != rows || Subtrahend.columns != columns){
            throw std::invalid_argument("Subtrahend Matrix does not have the same dimensions");
        }
        return *this += (Subtrahend * -1.0);
    }

    Matrix operator*(const Matrix& Factor) const{
        //Mat(a) * Mat(b) -> a.cols = b.rows
        if(columns != Factor.rows){
            throw std::invalid_argument("You can not multiply matrixes where first matrix rows != second matrix columns");
        }
        Matrix temp(rows, Factor.columns);
        //row 1 all cols  * all rows col 1
        for(size_t index{}; index < rows; index++){
            for(size_t index2{}; index2 < Factor.columns; index2++){
                double ans = 0;
                for(size_t index3{}; index3 < columns; index3++){
                    ans += (get(index, index3) * Factor.get(index3, index2));
                }
                temp.append(index, index2, ans);
            }
        }
        return temp;
    }

    Matrix& operator*=(const Matrix& Factor){
        *this = *this * Factor;  // Don't need the temp variable
        return *this;
    }

    Matrix T()const{
        Matrix answer(columns, rows);
        for(size_t row{}; row < rows; row++){
            for(size_t col{}; col < columns; col++){
                answer.append(col, row, data[row * columns + col]);
            }
        }
        return answer;
    }
    
    /*We could do cofactor expansion where we just go A*det(below) - B det(below) and stuff (really easy recursive implementation) but thats O(N!)
    So, let's instead learn about LU decompositon real quick and then implement that
    Okay so I implemented a really rudimentary version of this but I don't have row swaps but :(
    */
    std::pair<Matrix, Matrix> LU_Decomposition() const{
        if(rows != columns){
            throw(std::invalid_argument("You need a square matrix to take a determinant"));
        }
        Matrix L(rows, columns);
        Matrix U = *this;
        for(size_t i{}; i < rows; i++){
            L.append(i, i, 1);
        }
        //we are going to solve for L basically and iterate via columns
        for(size_t col{}; col < columns; col++){
            if(U.get(col, col)  == 0){
                throw std::runtime_error("LU decomposition failed because matrix is singular");
            }
            for(size_t row{col+1}; row < rows; row++){
                //above is 1 so we get that d/a so we want U[row, col]/u[row, row]
                L.append(row, col, (U.get(row, col) / U.get(col, col))); // then change u's cur row
                //pivot's row gets changed
                for(size_t U_COL{col}; U_COL < columns; U_COL++){
                    //get row,ucol -= whatever we just appened*u(col, U_COL) [row is wrong becuz we start row one more down]
                    U.append(row, U_COL, (U.get(row, U_COL) - L.get(row, col) * U.get(col,U_COL)));
                }
            }
        }
        return {L, U};
    }

    double det() const{
        if(rows != columns){
            throw(std::invalid_argument("You need a square matrix to take a determinant"));
        }
        try {
            Matrix U = LU_Decomposition().second;
            double determinant = 1;
            for(size_t i{}; i < rows; i++){
                determinant *= U.get(i, i);
            }
            return determinant;
        } catch(const std::runtime_error& e) {
            return 0.0;
        }
    }

    Matrix minor(size_t remove_row, size_t remove_col) const{
        if(remove_row >= rows){
            throw(std::invalid_argument("The row is greater than total rows"));
        }
        if(remove_col >= columns){
            throw(std::invalid_argument("The column is greater than total columns"));
        }
        Matrix temp(rows - 1, columns - 1);
        for(size_t row{}; row < rows; row++){
            for(size_t col{}; col < columns; col++){
                //if we get to remove_row or remove_col we need to subtract one
                if(row == remove_row || col == remove_col){
                    continue;
                }
                temp.append((row > remove_row ? row-1 : row), (col > remove_col ? col-1 : col), get(row, col));
            }
        }
        return temp;
    }

    Matrix cof() const{
        Matrix cofactor(rows, columns);
        for(size_t row{}; row < rows; row++){
            for(size_t col{}; col < columns; col++){
                double sign = (double) pow(-1.0, (row & 1) + (col & 1));
                double determinant = minor(row, col).det();
                cofactor.append(row, col, sign * determinant);
            }
        }
        return cofactor;
    }

    Matrix adj() const{
        //Cofactor matrix
        Matrix temp = cof();
        return temp.T();
    }

    Matrix inv() const{
        if(rows != columns){
            throw(std::invalid_argument("You need a square matrix to have an inverse"));
        }
        //1/det * adj(Matrix) -> check if det != 0 
        double determinant = det();
        if(determinant == 0){
            throw std::runtime_error("Matrix is not invertible as determinant is zero");
        }
        return ((1 / determinant) * adj());
    }

    void print() const {
        for(size_t i = 0; i < rows; i++) {
            for(size_t j = 0; j < columns; j++) {
                std::cout << get(i, j) << " ";
            }
            std::cout << std::endl;
        }
    }

    static Matrix lookAt(const Vec3D& eye, const Vec3D& focus, const Vec3D& up){
        //Let F = focus - eye
        Vec3D F = focus - eye;
        //Normalize it 
        F = F.normal();
        Vec3D normalUP = up.normal();
        Vec3D s = F.cross(normalUP);
        Vec3D u = s.normal().cross(F);
        Matrix ans(4, 4);
        ans.append(0,0,s.x());
        ans.append(0,1,s.y());
        ans.append(0,2,s.z());
        ans.append(0,3,-1*(s*eye));
        ans.append(1,0,u.x());
        ans.append(1,1,u.y());
        ans.append(1,2,u.z());
        ans.append(1,3,-1*(u*eye));
        ans.append(2,0,-1*F.x());
        ans.append(2,1,-1*F.y());
        ans.append(2,2,-1*F.z());
        ans.append(2,3,F*eye);
        ans.append(3,0,0);
        ans.append(3,1,0);
        ans.append(3,2,0);
        ans.append(3,3,1);
        return ans;
    }

};

#endif