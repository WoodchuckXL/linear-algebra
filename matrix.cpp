#include "matrix.h"

#include <sstream>
#include <iostream>
#include <iomanip>

Matrix::Matrix(int m, int n) : matx(n, Vector(m))
{
    if (n < 1) throw new std::out_of_range("You cannot create a matrix with <1 columns");
    nRows = m;
    nCols = n;
}
Matrix::Matrix(std::initializer_list<std::initializer_list<double>> arr)
{
    nCols = arr.size();
    if (nCols < 1) throw new std::out_of_range("You cannot create a matrix with <1 columns");
    nRows = arr.begin()->size();
    for (auto itr = arr.begin(); itr != arr.end(); ++itr)
    {
        matx.emplace_back(*itr);
    }
}
Matrix::Matrix(std::string matxFile)
{
    //TODO
    (void)matxFile;
}
Matrix::Matrix(const Matrix &other)
{
    nRows = other.nRows;
    nCols = other.nCols;
    for (int c = 0; c < nCols; ++c)
    {
        matx.emplace_back(other.getCol(c));
    }
}
Matrix::~Matrix()
{
    
}
Matrix Matrix::identity(int n)
{
    Matrix out(n,n);
    for (int i = 0; i < n; ++i)
    {
        out[i][i] = 1;
    }
    return out;
}

// Operators
Matrix Matrix::operator=(const Matrix &other)
{
    if (sameSizeAs(other)) 
    {
        for (int c = 0; c < nCols; ++c)
        {
            matx[c] = other.getCol(c);
        }
    } else 
    {
        matx.clear();
        for (int c = 0; c < nCols; ++c)
        {
            matx.emplace_back(other.getCol(c));
        }
    }
    return *this;
}
bool Matrix::operator==(const Matrix &other) const
{
    if (!sameSizeAs(other)) return false;
    for (int c = 0; c < nCols; ++c)
    {
        for (int r = 0; r < nRows; ++r)
        {
            if (other.at(c,r) != at(c,r)) return false;
        }
    }
    return true;
}
Matrix Matrix::operator+(const Matrix &other) const
{
    if (!sameSizeAs(other)) throw new std::runtime_error("Invalid matrix addition: matrices must have equal size");

    Matrix out(*this);
    for (int c = 0; c < nCols; ++c)
    {
        for (int r = 0; r < nRows; ++r)
        {
            out[c][r] += other.at(c,r);
        }
    }
    return out;
}
Matrix &Matrix::operator+=(const Matrix &other)
{
    if (!sameSizeAs(other)) throw new std::runtime_error("Invalid matrix addition: matrices must have equal size");

    for (int c = 0; c < nCols; ++c)
    {
        for (int r = 0; r < nRows; ++r)
        {
            matx[c][r] += other.at(c,r);
        }
    }
    return *this;
}
Vector Matrix::operator*(const Vector &x) const
{
    if (x.size() != nCols) throw new std::runtime_error("Invalid matrix Vector multiplication: vector size must equal matrix width");

    Vector out(nRows);
    for (int c = 0; c < nCols; ++c)
    {
        out += matx[c]*x.at(c);
    }
    return out;
}
Matrix Matrix::operator*(const Matrix &B) const
{
    if (this->nCols != B.nRows)
        throw new std::runtime_error("Invalid Matrix multiplication: width A must match height B");
    Matrix out(nRows, B.nCols);
    for (int r = 0; r < nRows; ++r)
    {
        for (int c = 0; c < B.nCols; ++c)
        {
            // Find value of cell
            for (int i = 0; i < nCols; ++i)
            {
                out[c][r] += this->getCol(i).at(r) * B.getCol(c).at(i);
            }   
        }
    }
    return out;
}
Vector &Matrix::operator[](int i)
{
    return matx[i];
}

// Properties
const Vector &Matrix::getCol(int c) const
{
    return matx[c];
}
double Matrix::at(int c, int r) const
{
    return matx[c].at(r);
}
int Matrix::findPivot(int row) const
{
    for (int c = 0; c < nCols; ++c)
    {
        if (matx[c].at(row) != 0)
        {
            return c;
        }
    }
    return nCols;
}
bool Matrix::isInREF() const
{
    int pivCol, lastPivCol = -1;
    for (int r = 0; r < nRows; ++r) 
    {
        pivCol = findPivot(r);
        if (lastPivCol == nCols && pivCol == nCols) continue;
        if (pivCol <= lastPivCol) 
        {
            return false;
        }
        lastPivCol = pivCol;
    }
    return true;
}
bool Matrix::isInRREF() const
{
    int pivCol, lastPivCol = -1;
    for (int r = 0; r < nRows; ++r) 
    {
        pivCol = findPivot(r);
        if (lastPivCol == nCols && pivCol == nCols) continue;
        if (pivCol <= lastPivCol) 
        {
            return false;
        }
        for (int j = 0; j < r; ++j)
        {
            if (at(pivCol, j) != 0) return false;
        }
        lastPivCol = pivCol;
    }
    return true;
}
int Matrix::numPivots() const
{
    int nPiv = 0;
    for (int r = 0; r < nRows; ++r) 
    {
        if (findPivot(r) < nCols) ++nPiv;
    }
    return nPiv;
}
bool Matrix::isInjective() const
{
    if (nCols > nRows) return false;
    int nPiv = 0;
    genericREF([](int,int){}, [&nPiv](int,double){++nPiv;}, [](int,int,double){});
    return nPiv == nCols;
}
bool Matrix::isSurjective() const
{
    if (nRows > nCols) return false;
    int nPiv = 0;
    genericREF([](int,int){}, [&nPiv](int,double){++nPiv;}, [](int,int,double){});
    return nPiv == nRows;
}
bool Matrix::isBijective() const
{
    if (nCols != nRows) return false;
    int nPiv = 0;
    genericREF([](int,int){}, [&nPiv](int,double){++nPiv;}, [](int,int,double){});
    return nPiv == nCols;
}
bool Matrix::sameSizeAs(const Matrix &other) const
{
    return (other.nCols == nCols && other.nRows == nRows);
}


// Methods
Matrix Matrix::getREF(Flags flags)
{
    return rowReductionFlagPreprocessor(flags);
}
Matrix Matrix::getREF(Matrix &aug, Flags flags)
{
    return rowReductionFlagPreprocessor(flags, 
                    {[&aug](int a,int b){aug.swapRow(a,b);}}, 
                    {[&aug](int a,double b){aug.scaleRow(a,b);}}, 
                    {[&aug](int a,int b,double c){aug.addRow(a,b,c);}});
}
Matrix Matrix::getInverse() const
{
    if (nCols != nRows) throw new std::runtime_error("Matrix is not invertible");
    Matrix inv = identity(nRows);
    int nPivs = 0;
    genericREF([&inv](int a,int b){inv.swapRow(a,b);}, 
               [&inv, &nPivs](int a,double b){inv.scaleRow(a,b); ++nPivs;}, 
               [&inv](int a,int b, double c){inv.addRow(a,b,c);}, true);
    if (nPivs != nRows) throw new std::runtime_error("Matrix is not invertible");
    return inv;
}
double Matrix::getDeterminant() const
{
    // Performs a guassian elimination "row reduction" on the matrix to bring
    // it into RREF, keeping track of every time where it swaps or scales rows
    // beacuse these operations change the determinant. 
    double d = 1;
    genericREF([&d](int,int){d *= -1;}, [&d](int,double c){d *= c;}, [](int,int,double){});
    // We invert each of these changes to recover the determinant.
    return 1/d;
}


// Row operations
void Matrix::swapRow(int r1, int r2) // r1 <- r2; r2 <- r1
{
    if (r1 >= nRows || r2 >= nRows) throw new std::out_of_range("Rows must be in matrix");
    if (r1 == r2) return;
    Vector temp(nCols);
    for (int c = 0; c < nCols; ++c)
    {
        temp[c] = matx[c][r2];
        matx[c][r2] = matx[c][r1];
        matx[c][r1] = temp[c];
    }
}
void Matrix::scaleRow(int r, double c) // r <- r * c
{
    if (r >= nRows) throw new std::out_of_range("Row must be in matrix");
    if (c == 1) return;
    for (int col = 0; col < nCols; ++col)
    {
        matx[col][r] *= c;
    }
}
void Matrix::addRow(int r, int r1, double c) // r <- r + r1 * c
{
    if (r >= nRows || r1 >= nRows) throw new std::out_of_range("Rows must be in matrix");
    if (c == 0) return;
    for (int col = 0; col < nCols; ++col)
    {
        matx[col][r] += matx[col][r1] * c;
    }
}

// Print methods
std::string Matrix::toString() const
{
    std::stringstream ss;
    for (int r = 0; r < nRows; ++r)
    {
        for (int c = 0; c < nCols; ++c)
        {
            ss << std::setw(print_width) << getCol(c).at(r) << " ";
        }
        if (r < nRows-1) ss << "\n";
    }
    return ss.str();
}
std::string Matrix::toString(const Matrix &aug) const
{
    if (nRows != aug.nRows)
        throw new std::out_of_range("Augmented matrix must have same number of rows as non-augmented matrix.");
    std::stringstream ss;
    for (int r = 0; r < nRows; ++r)
    {
        for (int c = 0; c < nCols; ++c)
        {
            ss << std::setw(print_width) << getCol(c).at(r) << " ";
        }
        ss << "| ";
        for (int c = 0; c < aug.nCols; ++c)
        {
            ss << std::setw(print_width) << aug.getCol(c).at(r) << " ";
        }
        if (r < nRows-1) ss << "\n";
    }
    return ss.str();
}
std::string Matrix::getLog() const
{
    return logStream.str();
}

// row reduction algorithm
Matrix Matrix::rowReductionFlagPreprocessor(Flags flags, 
                    const std::vector<std::function<void(int,int)>> swapL, 
                    const std::vector<std::function<void(int,double)>> scaleL, 
                    const std::vector<std::function<void(int,int,double)>> addL)
{
    std::vector<std::function<void(int,int)>> swapList = swapL;
    std::vector<std::function<void(int,double)>> scaleList = scaleL;
    std::vector<std::function<void(int,int,double)>> addList = addL;

    // Process flags to add elements to the function lists
    if (flags & Flags::LogOp) {
        swapList.push_back([this](int a,int b){this->logStream << "r" << a << " <-> r" << b << std::endl;});
        scaleList.push_back([this](int a,double b){this->logStream << "r" << a << " <- r" << a << " * " << b << std::endl;});
        addList.push_back([this](int a,int b,double c){this->logStream << "r" << a << " <- r" << a << " + r" << b << " * " << c << std::endl;});
    }

    // Compile lists into functions that call every element
    std::function<void(int,int)> swap = [&swapList](int a,int b) {
        for (auto itr = swapList.begin(); itr != swapList.end(); ++itr) (*itr)(a,b);
    };
    std::function<void(int,double)> scale = [&scaleList](int a,double b) {
        for (auto itr = scaleList.begin(); itr != scaleList.end(); ++itr) (*itr)(a,b);
    };
    std::function<void(int,int,double)> add = [&addList](int a,int b,double c) {
        for (auto itr = addList.begin(); itr != addList.end(); ++itr) (*itr)(a,b,c);
    };

    return genericREF(swap, scale, add, flags & Flags::RREF);
}

Matrix Matrix::genericREF(std::function<void(int,int)> swap, 
                          std::function<void(int,double)> scale, 
                          std::function<void(int,int,double)> add, bool toRREF) const
{
    Matrix matRREF(*this);

    std::vector<int> pivots; // The pivot at index i in this array is at location (pivots[i], i) in the matrix
    double temp;

    // For each column, try to find a row with a pivot and 
    // use it to eliminate the other elements in that column
    for (int c = 0; c < nCols; ++c) 
    {
        int pivotRow = -1; // Find a row with a nonzero element
        for (int r = pivots.size(); r < nRows; ++r) {
            if (matRREF[c][r] != 0) { // Found pivot
                pivotRow = r;
                pivots.push_back(c);
                break;
            }
        }
        if (pivotRow == -1) continue; // There is no pivot in this column, try next

        // Reduce pivot to 1 then move it to the ith pivot spot
        temp = 1/matRREF[c][pivotRow];
        matRREF.scaleRow(pivotRow, temp);
        scale(pivotRow, temp);
        if (pivotRow != (int)(pivots.size()-1))
        {
            matRREF.swapRow(pivotRow, pivots.size()-1);
            swap(pivotRow, pivots.size()-1);
        }
        pivotRow = pivots.size()-1;
        
        // For all non pivot columns remove value in pivot
        for (int r = pivots.size(); r < nRows; ++r)
        {
            temp = -matRREF[c][r];
            if (temp != 0)
            {    
                matRREF.addRow(r, pivotRow, temp);
                add(r, pivotRow, temp);
            }
        }
    }
    if (!toRREF) return matRREF; 

    // Continue to RREF
    for (int i = pivots.size()-1; i >= 0; --i)
    {
        for (int j = i-1; j >= 0; --j)
        {
            temp = -matRREF[pivots[i]][j];
            if (temp != 0)
            {   
                matRREF.addRow(j, i, temp);
                add(j,i,temp);
            }
        }
    }

    return matRREF;
}