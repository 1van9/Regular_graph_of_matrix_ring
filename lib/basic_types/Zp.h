#include<iostream>
// for working with numbers modulo p

template<long long P>
struct Z {
    Z() : x(0) {}
    Z(long long i) : x(i % P) {
        if (x < 0)
            x += P;
    }
    Z(const Z & a) : x(a.x) {}
    Z operator = (const Z & a) {
        x = a.x;
        return a;
    }
    Z operator + (const Z & other) const {
        return Z(x + other.x);
    }
    Z operator - (const Z & other) const {
        return Z(x - other.x);
    }
    Z operator += (const Z & other) {
        x += other.x;
        if (x >= P)
            x -= P;
        return *this;
    }
    Z operator -= (const Z & other) {
        x -= other.x;
        if (x < 0)
            x += P;
        return *this;
    }
    Z operator * (const Z & other) const {
        return Z(x * other.x);
    }
    Z operator *= (const Z & other) {
        x *= other.x;
        x %= P;
        return *this;
    }
    Z operator ^ (long long k) const {
        if (k == 0)
            return Z(1);
        Z val = ((*this) * (*this)) ^ (k >> 1LL);
        if (k & 1LL)
            return val * (*this);
        else
            return val; 
    }
    Z operator / (const Z & other) const {
        return (*this) * (other ^ (P - 2));
    }
    Z operator /= (const Z & other) {
        *this = (*this / other);
        return (*this);
    }
    operator bool() const {
        return x;
    }
    
    long long x;
};

template<long long P>
std::istream & operator >> (std::istream & in, Z<P> & a) {
    in >> a.x;
    a.x %= P;
    if (a.x < 0)
        a.x += P;
    return in;
} 

template<long long P>
std::ostream & operator << (std::ostream & out, const Z<P> & a) {
    out << a.x;
    return out;
}

template<long long P>
void swap(Z<P> & a, Z<P> & b) {
    a.x ^= b.x;
    b.x ^= a.x;
    a.x ^= b.x;
}
