#pragma ones

#include <algorithm>
#include "basic_types/matrix.h"
#include "basic_types/Zp.h"
#include "basic_types/rational.h"
#include "basic_types/poly.h"
#include "basic_types/integers.h"
#include <set>

using Q = Frac<long long>;
using Qll = Frac<Integer>;
using Z3 = Z<3>;
using Rf = Frac<Poly<Q>>;
using MatRf = Matrix<Rf>;
using MatQ = Matrix<Q>;
using MatN = Matrix<Integer>;
using Mat3 = Matrix<Z3>;
using Mat = Matrix<long long>;
using CliqueRf = vec<MatRf>;
using CliqueQ = vec<MatQ>;
using Clique3 = vec<Mat3>;


Rf x({Q(0), Q(1)});

MatQ substitute_point(const MatRf & a, const Q & v) {
    MatQ b(a.n, a.m);
    for (int i = 0; i < a.n; i++) {
        for (int j = 0; j < a.m; j++) {
            b[i][j] = a[i][j].fi(v) / a[i][j].se(v);
        }
    }
    return b;
}

CliqueQ substitute_point(const CliqueRf & a, const Q & v) {
    CliqueQ b(a.size());
    for (int i = 0; i < a.size(); i++) {
        b[i] = substitute_point(a[i], v);
    }
    return b;
}

template<typename T>
void print(vec<Matrix<T>> & a) {
    std::cout << "print matrixes : " << std::endl; 
    for (auto e : a)
        std::cout << e << std::endl;
}

Rf from_poly_to_rf(const Poly<int> & x) {
    vec<Q> coefs;
    for (auto a : x.coefficients)
        coefs.emplace_back(a);
    Rf y(Poly<Q>(coefs), Poly<Q>(Q(1)));
    return y;
}

MatRf from_poly_to_rf(const Matrix<Poly<int>> & x) {
    MatRf y(x.n, x.m);
    for (int i = 0; i < x.n; i++)
        for (int j = 0; j < x.m; j++)
            y[i][j] = from_poly_to_rf(x[i][j]);
    return y;
}

template<typename T>
void generate_small_matrices_rec(int i, int j, int N, Matrix<T> & curr_matrix, const vec<T>& numbers, vec<Matrix<T>> & matrices) {
    if (j == N)   //shift
        i++, j = 0;
    if (i == N) {
        if (curr_matrix.GL())
            matrices.push_back(curr_matrix);
        return;
    }
    for (auto number : numbers) {
        curr_matrix[i][j] = number;
        generate_small_matrices_rec(i, j + 1, N, curr_matrix, numbers, matrices);
    }
}

template<typename T>
vec<Matrix<T>> gen_matrices(const vec<T> & nums, int N) {
    Matrix<T> curr(N, N);
    vec<Matrix<T>> allm;
    generate_small_matrices_rec(0, 0, N, curr, nums, allm);
    return allm;
}

template<typename T>
bool isomorphic_subgraphs_2_by_2(const vec<Matrix<T>>& subg1, const vec<Matrix<T>>& subg2) {
    if (subg1.size() != subg2.size())
        return false;
    int n = subg1.size();
    vec<int> p(n);
    for (int i = 0; i < n; i++)
        p[i] = i;
    bool ok = false;
    // A_i = U B_{p_i} V
    // U^{-1} = B_{p_i} V (A_i)^-1   (1)

    // V = (x y)
    //     (z t)
    // Have n ways, how to express U, in term of x, y, z, t
    std::set<Matrix<T>> ways; // possible linear equasions on coefficients x, y, z, t
    do {
        vec<Matrix<T>> M; // express U coefficients in term of x, y, z, t 
        for (int i = 0; i < n; i++) {
            Matrix<T> A = subg2[p[i]], B = subg1[i].rev(); 
            Matrix<T> U_coefficients({
                {A[0][0] * B[0][0], A[0][0] * B[1][0], A[0][1] * B[0][0], A[0][1] * B[1][0]},
                {A[1][0] * B[0][0], A[1][0] * B[1][0], A[1][1] * B[0][0], A[1][1] * B[1][0]},
                {A[0][0] * B[0][1], A[0][0] * B[1][1], A[0][1] * B[0][1], A[0][1] * B[1][1]},
                {A[1][0] * B[0][1], A[1][0] * B[1][1], A[1][1] * B[0][1], A[1][1] * B[1][1]}
            }); 
            M.push_back(U_coefficients);
        }
        Matrix<T> linear_equasions((n - 1) * M[0].size(), 4); // equasions on x, y, z, t
        for (int i = 1; i < n; i++) {
            for (int j = 0; j < M[i].size(); j++) {
                for (int k = 0; k < M[i][j].size(); k++) {
                    // from U_0 - U_i = 0, where U_i = B_{p_i} V (A_i)^-1, we have:
                    linear_equasions[(i - 1) * (M[i].size()) + j][k] = M[0][j][k] - M[i][j][k]; }
            }
        }
        linear_equasions.Gause();
        ways.insert(linear_equasions);
    } while (next_permutation(p.begin(), p.end()));

    // the same for tansposition
    do {
        vec<Matrix<T>> M;
        for (int i = 0; i < n; i++) {
            Matrix<T> A = subg2[p[i]], B = (subg1[i].t()).rev();
            Matrix<T> U_coefficients({
                {A[0][0] * B[0][0], A[0][0] * B[1][0], A[0][1] * B[0][0], A[0][1] * B[1][0]},
                {A[1][0] * B[0][0], A[1][0] * B[1][0], A[1][1] * B[0][0], A[1][1] * B[1][0]},
                {A[0][0] * B[0][1], A[0][0] * B[1][1], A[0][1] * B[0][1], A[0][1] * B[1][1]},
                {A[1][0] * B[0][1], A[1][0] * B[1][1], A[1][1] * B[0][1], A[1][1] * B[1][1]}
            }); 
            M.push_back(U_coefficients);
        }
        Matrix<T> linear_equasions((n - 1) * M[0].size(), 4);
        for (int i = 1; i < n; i++) {
            for (int j = 0; j < M[i].size(); j++) {
                for (int k = 0; k < M[i][j].size(); k++) {
                    linear_equasions[(i - 1) * (M[i].size()) + j][k] = M[0][j][k] - M[i][j][k];
                }
            }
        }
        linear_equasions.Gause();
        ways.insert(linear_equasions);
    } while (next_permutation(p.begin(), p.end()));

    // check is there non zero solutions of x, y, z, t, which give V from GL_2

    for (auto lin_eq : ways) {
        if (lin_eq[3][3] == T(1)) {
            // 1 0 0 0
            // 0 1 0 0 
            // 0 0 1 0
            // 0 0 0 1
            continue;
        }
        if (!lin_eq[2][2] && lin_eq[2][3] == T(1)) {
            //     0 0           0
            // 0 0 1 0     0 1 * 0
            // 0 0 0 1     0 0 0 1
            if (lin_eq[1][2] == T(1) && !lin_eq[1][1])
                continue;
            if (!lin_eq[1][2])
                continue;
            return true;
        }
        if (lin_eq[2][2] == T(1)) {
            // 1 0 0 *
            // 0 1 0 *
            // 0 0 1 *
            T det = lin_eq[0][3] - lin_eq[1][3] * lin_eq[2][3];
            if (det) {
                return true;
            }
            continue;
        }
        if (!lin_eq[0][0]) {
            // 0 1 0 *     0 1 * 0     0 1 * *     0 0 1 0     0 0 1 0    0 0 0 1    0 0 0 0
            // 0 0 1 *     0 0 0 1     0 0 0 0     0 0 0 1     0 0 0 0    0 0 0 0    0 0 0 0
            if (!lin_eq[1][2] && lin_eq[1][3] == T(1)) {
                if (lin_eq[0][1] == T(1) && !lin_eq[0][2]) {
                    return true;
                }
                continue;
            }
            return true;
        }
        // 1 0 * *     1 * 0 *     1 * * 0   1 * * *
        // 0 1 * *     0 0 1 *     0 0 0 1   0 0 0 0
        if (lin_eq[1][1] == T(1)) {
            // 1 0 * *
            // 0 1 * * 
            if (!lin_eq[0][3] && !lin_eq[1][2] && lin_eq[0][2] == lin_eq[1][3]) {
                continue;
            }
            return true;
        }
        if (lin_eq[1][2] == T(1)) {
            // 1 * 0 *
            // 0 0 1 *
            if (!lin_eq[0][1] && (lin_eq[0][3] == lin_eq[1][3])) {
                continue;
            }
            return true;
        }
        return true;

    }
    return false;
}

namespace addition {
    bool only_R = false;
    bool case_of_Q = false;

    template<typename T>
    vec<T> solution_of_quadratic_eq(T A, T B, T C, char var) {
        if (!A) {
            if (!B) {
                return {};
            }
            return {-C / B};
        }
        T D = B * B - (A * C * T(4));
        // var = -B +- sqrt(D) / 2A
        if (!D) {
            return {-B / (A + A)};
        }
        if (D < T(0)) {
            return {};
        }
        T D_ = sqrt(D);
        if (D_ * D_ == D) {
            T x1 = (-B + D_) / (A + A);
            T x2 = (-B - D_) / (A + A);
            return {x1, x2};
        } else {
            if (case_of_Q) {
                std::cout << "Not additionable in Q but maybe in R" << std::endl;
                only_R = true;
            }
            return {};
        }
    }

    template<typename T>
    inline vec<T> express_through_x(T y0, T y1, T z0, T z1, T t0, T t1, const Matrix<T> & a) {
        // x (t0 + t1x) - (y0 + y1x)(z0 + z1x) + a[1][1]x - a[1][0](y0 - y1x) - a[0][1](z0 + z1x) + a[0][0](t0 + t1x) + det(A) = 0
        T A = t1 - y1 * z1;
        T B = t0 - y1 * z0 - z1 * y0 + a[1][1] - a[1][0] * y1 - a[0][1] * z1 + a[0][0] * t1;
        T C = - y0 * z0 - a[1][0] * y0 - a[0][1] * z0 + a[0][0] * t0 + a.det();
        // Ax^2 + Bx + C
        return solution_of_quadratic_eq(A, B, C, 'x');
    }

    template<typename T>
    inline vec<T> express_through_y(T x0, T x1, T z0, T z1, T t0, T t1, const Matrix<T> & a) {
        // (x0 + x1y) (t0 + t1y) - y(z0 + z1y) + a[1][1](x0 + x1y) - a[1][0]y - a[0][1](z0 + z1y) + a[0][0](t0 + t1y) + det(A) = 0
        T A = x1 * t1 - z1;
        T B = t1 * x0 + x1 * t0 - z0 + a[1][1] * x1 - a[1][0] - a[0][1] * z1 + a[0][0] * t1;
        T C = x0 * t0 + a[1][1] * x0 - a[0][1] * z0 + a[0][0] * t0 + a.det();
        // Ay^2 + By + C
        return solution_of_quadratic_eq(A, B, C, 'y');
    }

    template<typename T>
    inline vec<T> express_through_z(T x0, T x1, T y0, T y1, T t0, T t1, const Matrix<T> & a) {
        // (x0 + x1z)(t0 + t1z) - (y0 + y1z)z + a[1][1](x0 + x1z) - a[1][0](y0 - y1z) - a[0][1]z + a[0][0](t0 + t1z) + det(A) = 0
        T A = -y1 + x1 * t1;
        T B = -y0 + x1 * t0 + t1 * x0 + a[1][1] * x1 - a[1][0] * y1 - a[0][1] + a[0][0] * t1;
        T C = x0 * t0 + a[1][1] * x0 - a[1][0] * y0 + a[0][0] * t0 + a.det();
        // Az^2 + Bz + C
        return solution_of_quadratic_eq(A, B, C, 'z');
    }

    template<typename T>
    inline vec<T> express_through_t(T x0, T x1, T y0, T y1, T z0, T z1, const Matrix<T> & a) {
        // (x0 + x1t)t  - (y0 + y1t)(z0 + z1t) + a[1][1](x0 + x1t) - a[1][0](y0 + y1t) - a[0][1](z0 + z1t) + a[0][0]t + det(A) = 0
        T A = -y1 * z1 + x1;
        T B = -z1 * y0 - y1 * z0 + x0 + a[1][1] * x1 - a[1][0] * y1 - a[0][1] * z1 + a[0][0];
        T C = -y0 * z0 + a[1][1] * x0 - a[1][0] * y0 - a[0][1] * z0  + a.det();
        // At^2 + Bt + C = 0
        return solution_of_quadratic_eq(A, B, C, 't');
    }

    template<typename T>
    inline vec<Matrix<T>> check_coefficients(vec<T> & xs, vec<T> & ys, vec<T> & zs, vec<T> & ts, const vec<Matrix<T>> & clique) {
        std::set<Matrix<T>> answer;
        for (auto x : xs)
            for (auto y : ys)
                for (auto z : zs)
                    for (auto t : ts) {
                        Matrix<T> A({{x, y}, {z, t}});
                        if (!A.det())
                            continue;
                        bool ans = true;
                        for (auto X : clique)
                            if (!A.Connect(X)) {
                                ans = false;
                                break; 
                            }
                        if (ans)
                            answer.insert(A);
                    }
        vec<Matrix<T>> ans(answer.begin(), answer.end());
        return ans;
    }

};

// addition K4 to K(1, 1, 1, 1, 2)|K(1, 1, 1, 1, 1)|K(1, 1, 1, 1, 0)
template<typename T>
vec<Matrix<T>> is_additionable_K4(const vec<Matrix<T>> & clique) {
    assert(clique.size() == 4);
    // a[0][0] a[0][1]
    // a[1][0] a[1][1]
    // equations:
    // xz - yt + a[1][1]x - a[1][0]y - a[0][1]z + a[0][0]t + det(A) = 0
    //              ___ x -    ___ y -    ___ z +    ___ t + ___    = 0
    //              ___ x -    ___ y -    ___ z +    ___ t + ___    = 0
    //              ___ x -    ___ y -    ___ z +    ___ t + ___    = 0
    Matrix<T> linear_equations(3, 5);
    T d = clique[0].det();
    for (int i = 1; i < 4; i++) {
        linear_equations[i - 1][0] = clique[i][1][1] - clique[0][1][1];
        linear_equations[i - 1][1] = clique[0][1][0] - clique[i][1][0];
        linear_equations[i - 1][2] = clique[0][0][1] - clique[i][0][1];
        linear_equations[i - 1][3] = clique[i][0][0] - clique[0][0][0];
        linear_equations[i - 1][4] = clique[i].det() - d;
    }
    // in clique this linear equasions always independent
    linear_equations.Gause();
    if (!linear_equations[2][0] && !linear_equations[2][1] &&
        !linear_equations[2][2] && !linear_equations[2][3]) {    
        // * * * * *     
        // * * * * *
        // 0 0 0 0 *
        return {};
    }
    
    int val = 3;
    for (int i = 0; i < 3; i++) {
        if (!linear_equations[i][i]) {
            val = i;
            break;
        }
    }
    vec<T> xs, ys, zs, ts;
    if (val == 0) {
        // 0 1 0 0 *
        // 0 0 1 0 *     val = 0
        // 0 0 0 1 *
        xs = addition::express_through_x(-linear_equations[0][4], T(0), 
                                         -linear_equations[1][4], T(0),
                                         -linear_equations[2][4], T(0),
                                        clique[0]);
        ys = {-linear_equations[0][4]};
        zs = {-linear_equations[1][4]};
        ts = {-linear_equations[2][4]};
    }
    if (val == 1) {     
        // 1 * 0 0 *
        // 0 0 1 0 *     val = 1
        // 0 0 0 1 *
        ys = addition::express_through_y(-linear_equations[0][4], -linear_equations[0][1], 
                                         -linear_equations[1][4], T(0),
                                         -linear_equations[2][4], T(0),
                                         clique[0]);
        for (auto el : ys) {
            xs.push_back(-linear_equations[0][4] - linear_equations[0][1] * el);
        }
        zs = {-linear_equations[1][4]};
        ts = {-linear_equations[2][4]};
    }
    if (val == 2) {
        // 1 0 -a 0 -c 
        // 0 1 -b 0 -d   t = e; z; y = d + bz; x = c + az;
        // 0 0  0 1 -e
        zs = addition::express_through_z(-linear_equations[0][4], -linear_equations[0][2],
                                         -linear_equations[1][4], -linear_equations[1][2],
                                         -linear_equations[2][4], T(0),
                                         clique[0]);
        for (auto z : zs) { 
            xs.push_back(-linear_equations[0][4] - linear_equations[0][2] * z);
            ys.push_back(-linear_equations[1][4] - linear_equations[1][2] * z);
        }
        ts.push_back(-linear_equations[2][4]);
    }
    if (val == 3) {
        // 1 0 0 -a -d
        // 0 1 0 -b -e   t; z = f + ct; y = e + bt; x = d + at;  
        // 0 0 1 -c -f
        ts = addition::express_through_t(-linear_equations[0][4], -linear_equations[0][3],
                                         -linear_equations[1][4], -linear_equations[1][3],
                                         -linear_equations[2][4], -linear_equations[2][3],
                                         clique[0]);
        for (auto t : ts) { 
            xs.push_back(-linear_equations[0][4] - linear_equations[0][3] * t);
            ys.push_back(-linear_equations[1][4] - linear_equations[1][3] * t);
            zs.push_back(-linear_equations[2][4] - linear_equations[2][3] * t);
        }
    }
    vec<Matrix<T>> M = addition::check_coefficients(xs, ys, zs, ts, clique);
    return M;
}

// how many ways to expand K5 to K(1, 1, 1, 1, 2)
template<typename T>
int expand(const vec<Matrix<T>> & clique) {
    int cnt = 0;
    for (int i = 0; i < 5; i++) {
        vec<Matrix<T>> a = clique;
        a.erase(a.begin() + i);
        vec<Matrix<T>> add = is_additionable_K4(a);
        if (add.size() > 1)
            cnt++;
    }
    return cnt;
}
