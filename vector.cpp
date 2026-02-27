#include "vector.h"
#include <stdexcept>
#include <sstream>
#include <format>

Vector::Vector(int n)
{
    if (n <= 0) throw new std::invalid_argument("Invalid vector size: n must be a positive integer");

    this->n = n;
    this->vec = new double[n];
    for (int i = 0; i < n; ++i)
    {
        vec[i] = 0;
    }
}
Vector::Vector(std::initializer_list<double> data)
{
    this->n = data.size();
    if (n < 1) throw new std::invalid_argument("Invalid vector size: n must be a positive integer");
    this->vec = new double[n];
    int i = 0;
    for (auto itr = data.begin(); itr != data.end(); ++itr)
    {
        vec[i++] = *itr;
    }
}

Vector::Vector(const Vector &other)
{
    this->n = other.n;
    this->vec = new double[n];
    for (int i = 0; i < n; ++i)
    {
        vec[i] = other.at(i);
    }
}

Vector::~Vector()
{
    delete[](vec);
}

// Operators
bool Vector::operator==(const Vector &other) const
{
    if (n != other.size()) return false;
    for (int i = 0; i < n; ++i)
    {
        if (vec[i] != other.at(i)) return false;
    }
    return true;
}

Vector &Vector::operator=(const Vector &other)
{
    if (&other == this) return *this;
    if (n != other.size())
    {
        delete[](vec); 
        n = other.size();
        vec = new double[n];
    }
    for (int i = 0; i < n; ++i)
    {
        vec[i] = other.at(i);
    }
    return *this;
}
// Vector addition only works for vectors of the same size
Vector Vector::operator+(const Vector &u) const
{
    if (n != u.size())
        throw new std::invalid_argument("Cannot add vectors of differing sizes");

    Vector out(*this);
    for (int i = 0; i < out.n; ++i)
    {
        out[i] += u.at(i);
    }
    return out;
}
Vector &Vector::operator+=(const Vector &u)
{
    if (n != u.size())
        throw new std::invalid_argument("Cannot add vectors of differing sizes");

    for (int i = 0; i < n; ++i)
    {
        vec[i] += u.at(i);
    }
    return *this;
}
Vector Vector::operator*(double c) const 
{
    Vector out(*this);
    for (int i = 0; i < out.n; ++i)
    {
        out[i] *= c;
    }
    return out;
}
Vector &Vector::operator*=(double c)
{
    for (int i = 0; i < n; ++i)
    {
        vec[i] *= c;
    }
    return *this;
}
double &Vector::operator[](int i)
{
    return vec[i];
}
double Vector::at(int i) const
{
    return vec[i];
}

// Properties
std::string Vector::toString(char flags, int digits) const
{
    (void)flags;
    (void)digits;
    std::stringstream ss;
    ss << "(";
    for (int i = 0; i < n; ++i)
    {
        ss << vec[i];
        if (i < n-1) ss << " ";
    }
    ss << ")";
    return ss.str();
}

int Vector::size() const
{
    return n;
}