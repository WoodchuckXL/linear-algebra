#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"
#include <vector>
#include <string>
#include <sstream>
#include <initializer_list>
#include <functional>

enum Flags : uint8_t {
    Empty = 0,
    RREF  = 1 << 0,
    LogOp = 1 << 1,
};

class Matrix
{
    public:
    // Constructors
    Matrix(int m, int n);
    Matrix(std::initializer_list<std::initializer_list<double>> arr);
    Matrix(std::string matxFile);
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
    Matrix getREF(Flags flags = Flags::Empty);
    Matrix getREF(Matrix &aug, Flags flags = Flags::Empty);
    Matrix getInverse() const;
    double getDeterminant() const;
    
    // Row operations
    void swapRow(int r1, int r2);
    void scaleRow(int r, double c);
    void addRow(int r, int r1, double c);

    std::string toString() const;
    std::string toString(const Matrix &aug) const;
    std::string getLog() const;

    
    private:
    // Matrix data
    int nCols, nRows;
    std::vector<Vector> matx;

    // Non-essential object data
    int print_width = 2;
    std::stringstream logStream;

    Matrix rowReductionFlagPreprocessor(Flags flags, 
                    const std::vector<std::function<void(int,int)>> swapL = {}, 
                    const std::vector<std::function<void(int,double)>> scaleL = {}, 
                    const std::vector<std::function<void(int,int,double)>> addL = {});
    Matrix genericREF(std::function<void(int,int)> swap, 
                      std::function<void(int,double)> scale, 
                      std::function<void(int,int,double)> add, bool toRREF = false) const;
    int numPivots() const;
};

#endif