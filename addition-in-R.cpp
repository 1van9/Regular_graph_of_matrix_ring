#include <algorithm>
#include <cmath>
#include <set>
#include <random>
#include "lib/matrix-tools.h"
using namespace std;


void generate_small_numbers(int C, int max_denominator, vec<Q> & numbers) {
    for (int denom = max_denominator; denom >= 1; denom--) {
        for (int num = -C * denom; num <= C * denom; num++) {
            Q new_f = Q(num, denom);
            if (!count(numbers.begin(), numbers.end(), new_f))
                numbers.push_back(new_f);
        }
    }
}

Q gen_rand_frac(mt19937 & gen) {
    int a = gen() % 10000, b = gen() % 10000;
    while (b == 0) b = gen() % 10000;
    return Q(a, b);
}

MatQ gen_rand_matrix(mt19937& gen, int N) {
    MatQ a(N, N);
    while (!a.det()) {
        for (int i = 0; i < N; i++) 
            for (int j = 0; j < N; j++) 
                a[i][j] = gen_rand_frac(gen);
    }
    return a;
}

MatQ gen_matrix_from_list(mt19937 & gen, const vec<MatQ> & all) {
    return all[gen() % all.size()];
}

void generate_R_clique_for_2x2(vector<MatQ> & clique, const vec<MatQ> & small_matrices, int last_index, mt19937 & gen) {
    if (clique.size() < 4) {
        while (last_index--) {
            MatQ new_matrix;
            // new_matrix = small_matrices[last_index];
            new_matrix = gen_matrix_from_list(gen, small_matrices);
            bool ok = true;
            for (auto el : clique) {
                if (!el.Connect(new_matrix)) {
                    ok = false; 
                    break;
                }
            }
            if (ok) {
                clique.push_back(new_matrix);
                generate_R_clique_for_2x2(clique, small_matrices, last_index, gen);
                clique.pop_back();
            }
        }
    } else {
        is_additionable_K4(clique);
        if (addition::only_R) {
            cout << "Win!" << endl;
            for (auto el : clique) {
                cout << el << endl;
            }
            exit(0);
        }
    }
}

int main() {
    int N = 2;

    // vec<Q> small_numbers;
    // generate_small_numbers(2, 4, small_numbers);
    
    // vec<MatQ> small_matrices = gen_matrixes<Q>(small_numbers, 2);
    addition::case_of_Q = true;
    addition::only_R = false;
    vec<MatQ> clique;
    mt19937 gen(845);
    // generate_clique_for_2x2(clique, small_matrices, 100000, gen);
    vec<MatQ> result = {
        MatQ({{Q(-1, 2), Q(-7, 4)}, 
              {Q(-1, 2), Q(-2, 3)}}),
        
        MatQ({{Q(-1, 1), Q(-7, 4)}, 
              {Q(0, 1), Q(-1, 2)}}),
         
        MatQ({{Q(1, 2), Q(1, 4)}, 
              {Q(1, 2), Q(2, 1)}}),
         
        MatQ({{Q(1, 2), Q(7, 4)}, 
              {Q(3, 4), Q(-1, 2)}})
    };
    MatQ linear_equations(3, 5);
    for (int i = 1; i < 4; i++) {
        linear_equations[i - 1][0] = result[i][1][1] - result[0][1][1];
        linear_equations[i - 1][1] = result[0][1][0] - result[i][1][0];
        linear_equations[i - 1][2] = result[0][0][1] - result[i][0][1];
        linear_equations[i - 1][3] = result[i][0][0] - result[0][0][0];
        linear_equations[i - 1][4] = result[i].det() - result[0].det();
    }
    linear_equations.Gause();
    cout << "x y z t " << endl;
    cout << linear_equations << endl;
    
    // 1 0 0 -a -d
    // 0 1 0 -b -e   t; z = f + ct; y = e + bt; x = d + at;  
    // 0 0 1 -c -f
    Q a = -linear_equations[0][4], b = -linear_equations[0][3]; 
    Q c = -linear_equations[1][4], d = -linear_equations[1][3]; 
    Q e = -linear_equations[2][4], f = -linear_equations[2][3]; 
    addition::express_through_t(a, b, c, d, e, f, result[0]);
    cout << "x = " << a << " + " << b << "t" << endl;
    cout << "y = " << c << " + " << d << "t" << endl;
    cout << "z = " << e << " + " << f << "t" << endl;
    // (a + bt)t - (c + dt)(e + ft) =/= 0
    // (b - df)t^2 + (a - cf - de)f - ce =/= 0
    cout << (b - d * f) << "t^2 + " << (a - c * f - d * e) << "t + " << c * e << " =/= 0" << endl; 
    addition::case_of_Q = false;
    addition::only_R = false;

}