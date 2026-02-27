#include "matrix.h"

#include <iostream>

int main(int argc, char *argv[])
{
    (void)argc; 
    (void)argv;

    Matrix I3 = Matrix::identity(3);


    Matrix A({{3,0,2}, {2,1,2}, {2,3,3}});
    Matrix B_inv({{3,1,1},{2,1,0},{1,0,0}});

    Matrix b({{-2,0,4}});
    Matrix A_inv = I3;

    Matrix A_prime = A.getRREF(A_inv);
    std::cout << A_prime.toString(A_inv) << std::endl;
}