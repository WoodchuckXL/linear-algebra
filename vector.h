#ifndef VECTOR_H
#define VECTOR_H

#include <string>
#include <initializer_list>

class Vector
{
    public:
    Vector(int n);
    Vector(std::initializer_list<double> data);
    Vector(const Vector &other);
    ~Vector();

    // Operators
    bool operator==(const Vector &other) const;
    Vector &operator=(const Vector &other);
    Vector operator+(const Vector &u) const;
    Vector &operator+=(const Vector &u);
    Vector operator*(double c) const;
    Vector &operator*=(double c);
    double &operator[](int i);
    double at(int i) const;

    // Properties
    std::string toString(char flags=0x00000000, int digits=3) const;
    int size() const;

    private:
    int n;
    double *vec;
};

#endif