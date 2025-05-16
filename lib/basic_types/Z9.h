#include<iostream>
#include<vector>

// Z3[x] / (x^2 + x + 2)
/*
3 = x
4 = x + 1
5 = x + 2
6 = 2x
7 = 2x + 1
8 = 2x + 2 
*/
// sum
/*
  0 1 2 3 4 5 6 7 8
0 0 1 2 3 4 5 6 7 8
1 1 2 0 4 5 3 7 8 6
2 2 0 1 5 3 4 8 6 7
3 3 4 5 6 7 8 0 1 2
4 4 5 3 7 8 6 1 2 0
5 5 3 4 8 6 7 2 0 1
6 6 7 8 0 1 2 3 4 5
7 7 8 6 1 2 0 4 5 3
8 8 6 7 2 0 1 5 3 4
*/

// mul
/*
3 = x
4 = x + 1
5 = x + 2
6 = 2x
7 = 2x + 1
8 = 2x + 2 
*/
/*
  0 1 2 3 4 5 6 7 8
0 0 0 0 0 0 0 0 0 0
1 0 1 2 3 4 5 6 7 8
2 0 2 1 6 8 7 3 5 4
3 0 3 6 7 1 4 5 8 2
4 0 4 8 1 5 6 2 3 7
5 0 5 7 4 6 2 8 1 3
6 0 6 3 5 2 8 7 4 1
7 0 7 5 8 3 1 4 2 6
8 0 8 4 2 7 3 1 6 5
*/

int sum_table[9][9] = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8}, 
    {1, 2, 0, 4, 5, 3, 7, 8, 6}, 
    {2, 0, 1, 5, 3, 4, 8, 6, 7}, 
    {3, 4, 5, 6, 7, 8, 0, 1, 2},
    {4, 5, 3, 7, 8, 6, 1, 2, 0},
    {5, 3, 4, 8, 6, 7, 2, 0, 1},
    {6, 7, 8, 0, 1, 2, 3, 4, 5},
    {7, 8, 6, 1, 2, 0, 4, 5, 3},
    {8, 6, 7, 2, 0, 1, 5, 3, 4}};

int mul_table[9][9] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 1, 2, 3, 4, 5, 6, 7, 8}, 
    {0, 2, 1, 6, 8, 7, 3, 5, 4}, 
    {0, 3, 6, 7, 1, 4, 5, 8, 2},
    {0, 4, 8, 1, 5, 6, 2, 3, 7},
    {0, 5, 7, 4, 6, 2, 8, 1, 3},
    {0, 6, 3, 5, 2, 8, 7, 4, 1},
    {0, 7, 5, 8, 3, 1, 4, 2, 6},
    {0, 8, 4, 2, 7, 3, 1, 6, 5}};
int rev[9] = {0, 1, 2, 4, 3, 7, 8, 5, 6};

struct Z9 {
    Z9() : x(0) {}
    Z9(long long i) : x(i) {}
    Z9(const Z9 & a) : x(a.x) {}
    Z9 operator = (const Z9 & a) {
        x = a.x;
        return a;
    }
    Z9 operator + (const Z9 & other) const {
        return Z9(sum_table[x][other.x]);
    }
    Z9 operator - (const Z9 & other) const {
        return (*this) + other;
    }
    Z9 operator - () const {
        return *this;
    }
    Z9 operator += (const Z9 & other) {
        *this = (*this) + other;
        return *this;
    }
    Z9 operator -= (const Z9 & other) {
        *this = (*this) + other;
        return *this;
    }
    Z9 operator * (const Z9 & other) const {
        return Z9(mul_table[x][other.x]);
    }
    Z9 operator *= (const Z9 & other) {
        *this = (*this) * other;
        return *this;
    }
    Z9 operator ^ (long long k) const {
        if (k == 0)
            return Z9(1);
        Z9 val = ((*this) * (*this)) ^ (k >> 1LL);
        if (k & 1LL)
            return val * (*this);
        else
            return val; 
    }
    Z9 operator / (const Z9 & other) const {
        return Z9(mul_table[x][rev[other.x]]);
    }
    Z9 operator /= (const Z9 & other) {
        *this = (*this / other);
        return (*this);
    }
    bool operator == (const Z9 & other) const {
        return x == other.x;
    }
    bool operator != (const Z9 & other) const {
        return x != other.x;
    }
    bool operator < (const Z9 & other) const {
        return x < other.x;
    }
    bool operator > (const Z9 & other) const {
        return x > other.x;
    }
    bool operator <= (const Z9 & other) const {
        return x <= other.x;
    }
    bool operator >= (const Z9 & other) const {
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

std::istream & operator >> (std::istream & in, Z9 & a) {
    in >> a.x;
    return in;
} 

std::ostream & operator << (std::ostream & out, const Z9 & a) {
    out << a.x;
    return out;
}

void swap(Z9 & a, Z9 & b) {
    a.x ^= b.x;
    b.x ^= a.x;
    a.x ^= b.x;
}

Z9 sqrt(Z9 & a) {
    // return Z9(rev[a.x]);
    for (int i = 0; i < 9; i++) {
        if (Z9(i) * Z9(i) == a)
            return Z9(i);
    }
    return Z9(0);
}
