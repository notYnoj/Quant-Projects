#include "matrix.hpp"
#include <stdexcept>
#include <float.h>
#include <cmath>

Matrix::Matrix(const size_t& _rows, const size_t& _columns) : rows(_rows), columns(_columns){
    data.resize(rows * columns);
}

void Matrix::append(size_t row, size_t col, double value) {
    if(row >= rows || col >= columns) {
        throw std::out_of_range("You are trying to insert an element that doesn't exist");
    }
    data[row * columns + col] = value;
}

double Matrix::get(size_t row, size_t col) {
    if(row >= rows || col >= columns) {
        throw std::out_of_range("Matrix indices out of range");
    }
    return data[row * columns + col];
}

double Matrix::get(size_t row, size_t col) const {
    if(row >= rows || col >= columns) {
        throw std::out_of_range("Matrix indices out of range");
    }
    return data[row * columns + col];
}

double& Matrix::operator()(size_t row, size_t col) {
    if(row >= rows || col >= columns) {
        throw std::out_of_range("Matrix indices out of range");
    }
    return data[row * columns + col];
}

const double& Matrix::operator()(size_t row, size_t col) const {
    if(row >= rows || col >= columns) {
        throw std::out_of_range("Matrix indices out of range");
    }
    return data[row * columns + col];
}

Matrix Matrix::operator+(const Matrix& Addend) const {
    if(Addend.rows != rows || Addend.columns != columns) {
        throw std::invalid_argument("Addend Matrix does not have the same dimensions");
    }
    Matrix temp(rows, columns);
    for(size_t index{}; index < rows * columns; index++) {
        temp.data[index] = data[index] + Addend.data[index];
    }
    return temp;
}

Matrix& Matrix::operator+=(const Matrix& Addend) {
    if(Addend.rows != rows || Addend.columns != columns) {
        throw std::invalid_argument("Addend Matrix does not have the same dimensions");
    }
    for(size_t index{}; index < rows * columns; index++) {
        data[index] += Addend.data[index];
    }
    return *this;
}

Matrix Matrix::operator*(const double& scalar) const {
    Matrix temp(rows, columns);
    for(size_t index{}; index < rows * columns; index++) {
        if(data[index] > (DBL_MAX / scalar)) {
            throw std::overflow_error("Overflow Error when attempting scalar multiplication");
        }
        temp.data[index] = data[index] * scalar;
    }
    return temp;
}

Matrix& Matrix::operator*=(const double& scalar) {
    for(size_t index{}; index < rows * columns; index++) {
        if(data[index] > DBL_MAX / scalar) {
            throw std::overflow_error("Overflow Error when attempting scalar multiplication");
        }
        data[index] *= scalar;
    }
    return *this;
}

Matrix operator*(double scalar, const Matrix& mat) {
    return mat * scalar;
}

Matrix Matrix::operator-(const Matrix& Subtrahend) const {
    if(Subtrahend.rows != rows || Subtrahend.columns != columns) {
        throw std::invalid_argument("Subtrahend Matrix does not have the same dimensions");
    }
    return (*this + (Subtrahend * -1.0));
}

Matrix& Matrix::operator-=(const Matrix& Subtrahend) {
    if(Subtrahend.rows != rows || Subtrahend.columns != columns) {
        throw std::invalid_argument("Subtrahend Matrix does not have the same dimensions");
    }
    return *this += (Subtrahend * -1.0);
}

Matrix Matrix::operator*(const Matrix& Factor) const {
    if(columns != Factor.rows) {
        throw std::invalid_argument("You can not multiply matrixes where first matrix rows != second matrix columns");
    }
    Matrix temp(rows, Factor.columns);
    for(size_t index{}; index < rows; index++) {
        for(size_t index2{}; index2 < Factor.columns; index2++) {
            double ans = 0;
            for(size_t index3{}; index3 < columns; index3++) {
                // Use const get() method here
                ans += (this->get(index, index3) * Factor.get(index3, index2));
            }
            temp.append(index, index2, ans);
        }
    }
    return temp;
}

Matrix& Matrix::operator*=(const Matrix& Factor) {
    *this = *this * Factor;
    return *this;
}

Matrix Matrix::T() const {
    Matrix answer(columns, rows);
    for(size_t row{}; row < rows; row++) {
        for(size_t col{}; col < columns; col++) {
            answer.append(col, row, data[row * columns + col]);
        }
    }
    return answer;
}

std::pair<Matrix, Matrix> Matrix::LU_Decomposition() const {
    if(rows != columns) {
        throw std::invalid_argument("You need a square matrix for LU decomposition");
    }
    Matrix L(rows, columns);
    Matrix U = *this;
    
    for(size_t i{}; i < rows; i++) {
        L.append(i, i, 1);
    }
    
    for(size_t col{}; col < columns; col++) {
        if(U.get(col, col) == 0) {
            throw std::runtime_error("LU decomposition failed because matrix is singular");
        }
        for(size_t row{col+1}; row < rows; row++) {
            L.append(row, col, (U.get(row, col) / U.get(col, col)));
            for(size_t U_COL{col}; U_COL < columns; U_COL++) {
                U.append(row, U_COL, (U.get(row, U_COL) - L.get(row, col) * U.get(col, U_COL)));
            }
        }
    }
    return {L, U};
}

double Matrix::det() const {
    if(rows != columns) {
        throw std::invalid_argument("You need a square matrix to take a determinant");
    }
    try {
        Matrix U = LU_Decomposition().second;
        double determinant = 1;
        for(size_t i{}; i < rows; i++) {
            determinant *= U.get(i, i);
        }
        return determinant;
    } catch(const std::runtime_error& e) {
        return 0.0;
    }
}

Matrix Matrix::minor(size_t remove_row, size_t remove_col) const {
    if(remove_row >= rows) {
        throw std::invalid_argument("The row is greater than total rows");
    }
    if(remove_col >= columns) {
        throw std::invalid_argument("The column is greater than total columns");
    }
    Matrix temp(rows - 1, columns - 1);
    for(size_t row{}; row < rows; row++) {
        for(size_t col{}; col < columns; col++) {
            if(row == remove_row || col == remove_col) {
                continue;
            }
            temp.append((row > remove_row ? row-1 : row), 
                        (col > remove_col ? col-1 : col), 
                        this->get(row, col));
        }
    }
    return temp;
}

Matrix Matrix::cof() const {
    Matrix cofactor(rows, columns);
    for(size_t row{}; row < rows; row++) {
        for(size_t col{}; col < columns; col++) {
            double sign = pow(-1.0, (row & 1) + (col & 1));
            double determinant = minor(row, col).det();
            cofactor.append(row, col, sign * determinant);
        }
    }
    return cofactor;
}

Matrix Matrix::adj() const {
    Matrix temp = cof();
    return temp.T();
}

Matrix Matrix::inv() const {
    if(rows != columns) {
        throw std::invalid_argument("You need a square matrix to have an inverse");
    }
    double determinant = det();
    if(determinant == 0) {
        throw std::runtime_error("Matrix is not invertible as determinant is zero");
    }
    return ((1 / determinant) * adj());
}

void Matrix::print() const {
    for(size_t i = 0; i < rows; i++) {
        for(size_t j = 0; j < columns; j++) {
            std::cout << this->get(i, j) << " ";
        }
        std::cout << std::endl;
    }
}

Matrix Matrix::lookAt(const Vec3D& eye, const Vec3D& focus, const Vec3D& up) {
    Vec3D F = focus - eye;
    F = F.normal();
    Vec3D normalUP = up.normal();
    Vec3D s = F.cross(normalUP);
    Vec3D u = s.normal().cross(F);
    
    Matrix ans(4, 4);
    ans.append(0, 0, s.x());
    ans.append(0, 1, s.y());
    ans.append(0, 2, s.z());
    ans.append(0, 3, -1.0 * (s * eye));
    ans.append(1, 0, u.x());
    ans.append(1, 1, u.y());
    ans.append(1, 2, u.z());
    ans.append(1, 3, -1.0 * (u * eye));
    ans.append(2, 0, -1.0 * F.x());
    ans.append(2, 1, -1.0 * F.y());
    ans.append(2, 2, -1.0 * F.z());
    ans.append(2, 3, F * eye);
    ans.append(3, 0, 0);
    ans.append(3, 1, 0);
    ans.append(3, 2, 0);
    ans.append(3, 3, 1);
    return ans;
}