#include<iostream>
#include<vector>

// sum
/*
  0 1 2 3
0 0 1 2 3
1 1 0 3 2
2 2 3 0 1
3 3 2 1 0
*/

// mul
/*
  0 1 2 3
0 0 0 0 0
1 0 1 2 3
2 0 2 3 1
3 0 3 1 2
*/

int sum_table[4][4] = {{0, 1, 2, 3}, {1, 0, 3, 2}, {2, 3, 0, 1}, {3, 2, 1, 0}};
int mul_table[4][4] = {{0, 0, 0, 0}, {0, 1, 2, 3}, {0, 2, 3, 1}, {0, 3, 1, 2}};
int rev[4] = {0, 1, 3, 2};

struct Z4 {
    Z4() : x(0) {}
    Z4(long long i) : x(i) {}
    Z4(const Z4 & a) : x(a.x) {}
    Z4 operator = (const Z4 & a) {
        x = a.x;
        return a;
    }
    Z4 operator + (const Z4 & other) const {
        return Z4(sum_table[x][other.x]);
    }
    Z4 operator - (const Z4 & other) const {
        return (*this) + other;
    }
    Z4 operator - () const {
        return *this;
    }
    Z4 operator += (const Z4 & other) {
        *this = (*this) + other;
        return *this;
    }
    Z4 operator -= (const Z4 & other) {
        *this = (*this) + other;
        return *this;
    }
    Z4 operator * (const Z4 & other) const {
        return Z4(mul_table[x][other.x]);
    }
    Z4 operator *= (const Z4 & other) {
        *this = (*this) * other;
        return *this;
    }
    Z4 operator ^ (long long k) const {
        if (k == 0)
            return Z4(1);
        Z4 val = ((*this) * (*this)) ^ (k >> 1LL);
        if (k & 1LL)
            return val * (*this);
        else
            return val; 
    }
    Z4 operator / (const Z4 & other) const {
        return Z4(mul_table[x][rev[other.x]]);
    }
    Z4 operator /= (const Z4 & other) {
        *this = (*this / other);
        return (*this);
    }
    bool operator == (const Z4 & other) const {
        return x == other.x;
    }
    bool operator != (const Z4 & other) const {
        return x != other.x;
    }
    bool operator < (const Z4 & other) const {
        return x < other.x;
    }
    bool operator > (const Z4 & other) const {
        return x > other.x;
    }
    bool operator <= (const Z4 & other) const {
        return x <= other.x;
    }
    bool operator >= (const Z4 & other) const {
        return x >= other.x;
    }
    operator bool() const {
        return x;
    }
    operator int() const {
        return x;
    }

    int x;
};

std::istream & operator >> (std::istream & in, Z4 & a) {
    in >> a.x;
    return in;
} 

std::ostream & operator << (std::ostream & out, const Z4 & a) {
    out << a.x;
    return out;
}

void swap(Z4 & a, Z4 & b) {
    a.x ^= b.x;
    b.x ^= a.x;
    a.x ^= b.x;
}

Z4 sqrt(Z4 & a) {
    return Z4(rev[a.x]);
    for (int i = 0; i < 4; i++) {
        if (Z4(i) * Z4(i) == a)
            return Z4(i);
    }
    return Z4(0);
}
