#include "matrix.h"

#include <iostream>

int main(int argc, char *argv[])
{
    (void)argc; 
    (void)argv;

    Matrix I3 = Matrix::identity(3);


    Matrix A({{0,1,0,2,0}, {2,0,2,0,3}, {2,0,3,0,5}, {0,2,0,6,0}, {2,0,4,0,8}});
    Matrix A_prime = A.getREF(Flags::LogOp);
    std::cout << A.toString() << std::endl << '\n';
    std::cout << A.getLog() << '\n';
    std::cout << A_prime.toString() << std::endl;
    std::cout << "det(A)=" << A.getDeterminant() << std::endl;
}