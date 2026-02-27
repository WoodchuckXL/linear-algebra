#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"
#include <vector>
#include <string>
#include <initializer_list>
#include <functional>

class Matrix
{
    public:
    // Constructors
    Matrix(int m, int n);
    Matrix(std::initializer_list<std::initializer_list<double>> arr);
    Matrix(const Matrix &other);
    ~Matrix();
    static Matrix identity(int n);

    // Operators
    Matrix operator=(const Matrix &other);
    bool operator==(const Matrix &other) const;
    Matrix operator+(const Matrix &other) const;
    Matrix &operator+=(const Matrix &other);
    Vector operator*(const Vector &x) const;
    Matrix operator*(const Matrix &B) const;
    Vector &operator[](int i);

    // Properties
    const Vector &getCol(int c) const;
    double at(int c, int r) const;
    int findPivot(int row) const;
    bool sameSizeAs(const Matrix &other) const;
    bool isInREF() const;
    bool isInRREF() const;
    bool isInjective() const;
    bool isSurjective() const;
    bool isBijective() const;

    // Methods
    Matrix getREF() const;
    Matrix getREF(Matrix &aug) const;
    Matrix getRREF() const;
    Matrix getRREF(Matrix &aug) const;
    Matrix getInverse() const;
    double getDeterminant() const;
    
    // Row operations
    void swapRow(int r1, int r2);
    void scaleRow(int r, double c);
    void addRow(int r, int r1, double c);

    std::string toString() const;
    std::string toString(const Matrix &aug) const;

    
    private:
    int nCols, nRows;
    int print_width = 2;
    // The matrix is stored as a an array of columns
    std::vector<Vector> matx;

    Matrix genericREF(std::function<void(int,int)> swap, 
                      std::function<void(int,double)> scale, 
                      std::function<void(int,int, double)> add, bool toRREF = false) const;
    int numPivots() const;
};

#endif