#include <algorithm>
#include <cmath>
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

void generate_clique_for_2x2(CliqueQ & clique, const vec<MatQ> & small_matrices, int last_index) {
    if (clique.size() < 4) {
        while (last_index--) {
            MatQ new_matrix = small_matrices[last_index];
            bool ok = true;
            for (auto el : clique) {
                if (!el.Connect(new_matrix)) {
                    ok = false; 
                    break;
                }
            }
            if (ok) {
                clique.push_back(new_matrix);
                generate_clique_for_2x2(clique, small_matrices, last_index);
                clique.pop_back();
            }
        }
    } else {
        cout << "minimal success" << endl;
        // a[0][0] a[0][1]
        // a[1][0] a[1][1]
        // equations:
        // xz - yt + a[1][1]x - a[1][0]y - a[0][1]z + a[0][0]t + det(A) = 0
        //              ___ x -    ___ y -    ___ z +    ___ t + ___    = 0
        //              ___ x -    ___ y -    ___ z +    ___ t + ___    = 0
        //              ___ x -    ___ y -    ___ z +    ___ t + ___    = 0
        MatQ linear_equations(3, 5);
        for (int i = 1; i < 4; i++) {
            linear_equations[i - 1][0] = clique[i][1][1] - clique[0][1][1];
            linear_equations[i - 1][1] = clique[0][1][0] - clique[i][1][0];
            linear_equations[i - 1][2] = clique[0][0][1] - clique[i][0][1];
            linear_equations[i - 1][3] = clique[i][0][0] - clique[0][0][0];
            linear_equations[i - 1][4] = clique[i].det() - clique[0].det();
        }
        // this linear equasions always independent
        linear_equations.Gause();
  
        if (!linear_equations[2][0] && !linear_equations[2][1] &&
            !linear_equations[2][2] && !linear_equations[2][3]) {    
            // 1 * 0 * *     
            // 0 0 1 * *   or something like this
            // 0 0 0 0 *
            cout << "No more than one solution" << endl;
            return;
        }

        // 1 0 3 0 *
        // 0 1 1 0 *
        // 0 0 0 1 *
        int val = 3;
        for (int i = 0; i < 3; i++) {
            if (!linear_equations[i][i]) {
                val = i;
                break;
            }
        }
        if (val == 0 || val == 1) {
            // 0 1 0 0 *
            // 0 0 1 0 *     val = 0
            // 0 0 0 1 *
            
            // 1 2 0 0 *
            // 0 0 1 0 *     val = 1
            // 0 0 0 1 *
            cout << "No more than one solution" << endl;
            return;
        }
        if (val == 2) {
            // 1 0 -a 0 -c 
            // 0 1 -b 0 -d   t = e; z; y = d + bz; x = c + az;
            // 0 0  0 1 -e

            // (az + c)e - (d + bz)z + clique[0][1][1](c + az) - clique[0][1][0](d + bz) - clique[0][0][1]z + clique[0][0][0]e + det(clique[0])
            // -b * z^2 + (ae - d + a clique[0][1][1] - b clique[0][1][0] - clique[0][0][1])t + ...
            Q a = -linear_equations[0][3];
            Q b = -linear_equations[1][3];
            Q c = -linear_equations[0][4];
            Q d = -linear_equations[1][4];
            Q e = -linear_equations[2][4];
            Q A = -d;
            Q B = a * e - d + a * clique[0][1][1] - b * clique[0][1][0] - clique[0][0][1];
            Q C = c * e + c * clique[0][1][1] - clique[0][1][0] * d - clique[0][0][0] * e + clique[0].det();
            // Az^2 + Bz + C
            Frac D = B * B - (Q(4, 1) * A * C);
            if (!A || !D) {
                cout << "No more than one solution" << endl;
                return;
            }
            cout << "Success" << endl;
            // cout << "equations : " << endl;
            // cout << linear_equations << endl;
            // cout << A << " z^2 + " << B << " z + " << C << endl;

            long long x = sqrt(D.fi), y = sqrt(D.se);
            if (x * x == D.fi && y * y == D.se) {
                Q D_ = Q(x, y);
                Q z1 = (D_ - B) / (Q(2, 1) * A);
                Q z2 = (Q(0, 1) - D_ - B) / (Q(2, 1) * A);
                Q x1 = c + z1 * a, y1 = d + z1 * b, t1 = e;
                Q x2 = c + z2 * a, y2 = d + z2 * b, t2 = e;
                if (!(x1 * t1 - y1 * z1) || !(x2 * t2 - y2 * z2)) {
                    cout << "Not now" << endl;
                    return;
                } else {
                    cout << "---Got it!---" << endl;
                    cout << clique[0] << endl << clique[1] << endl;
                    cout << clique[2] << endl << clique[3] << endl;
                    cout << "-----and-----" << endl;
                    cout << x1 << " " << y1 << endl;
                    cout << z1 << " " << t1 << endl;
                    cout << endl;
                    cout << x2 << " " << y2 << endl;
                    cout << z2 << " " << t2 << endl;
                    cout << "-------------";
                    cout << endl;
                    exit(0);
                    return;
                }
            } else {
                cout << "Maybe" << endl;
                return;
            }
        }
        // 1 0 0 -a -d
        // 0 1 0 -b -e   t; z = f + ct; y = e + bt; x = d + at;  
        // 0 0 1 -c -f
        
        // (at + d)t - (e + bt)(f + ct) + clique[0][1][1](d + at) - clique[0][1][0](e + bt) - clique[0][0][1](f + ct) + clique[0][0][0]t + det(clique[0])
        // (a - bc) * t^2 + (d - ec - bf + a clique[0][1][1] - b clique[0][1][0] - c clique[0][0][1] + clique[0][0][0])t + ...
        Q a = -linear_equations[0][3];
        Q b = -linear_equations[1][3];
        Q c = -linear_equations[2][3];
        Q d = -linear_equations[0][4];
        Q e = -linear_equations[1][4];
        Q f = -linear_equations[2][4];
        Q A = a - (c*b);
        Q B = d - (e * c) - (b * f) + (a * clique[0][1][1]) - (b * clique[0][1][0]) - (c * clique[0][0][1]) + clique[0][0][0];
        Q C = -(e * f) + (clique[0][1][1] * d) - (clique[0][1][0] * e) - (clique[0][0][1] * f) + clique[0].det();
        //At^2 + Bt + C
        Q D = B * B - (Q(4, 1) * A * C);
        if (!A || !D) {
            cout << "No more than one solution" << endl;
            return;
        }
        cout << "Success" << endl;
        // cout << "equations : " << endl;
        // cout << linear_equations << endl;
        // cout << A << " x^2 + " << B << " x + " << C << endl;

        Q D_ = sqrt(D);
        if (D_ * D_ == D) {
            Q t1 = (D_ - B) / (Q(2, 1) * A);
            Q t2 = (-D_ - B) / (Q(2, 1) * A);
            Q z1 = f + c * t1, y1 = e + b * t1, x1 = d + a * t1;
            Q z2 = f + c * t2, y2 = e + b * t2, x2 = d + a * t2;
            if (!(x1 * t1 - y1 * z1) || !(x2 * t2 - y2 * z2)) {
                cout << "No more than one solution" << endl;
                return;
            } else {
                cout << "---Got it!---" << endl;
                cout << clique[0] << endl << clique[1] << endl;
                cout << clique[2] << endl << clique[3] << endl;
                cout << "-----and-----" << endl;
                cout << x1 << " " << y1 << endl;
                cout << z1 << " " << t1 << endl;
                cout << endl;
                cout << x2 << " " << y2 << endl;
                cout << z2 << " " << t2 << endl;
                cout << "-------------" << endl;
                exit(0);
            }
        } else {
            cout << "Maybe" << endl;
            return;
        }
        return;
    }
}

int main() {
    int N = 2, C = 2, max_denom = 4;

    vec<Q> small_numbers;
    generate_small_numbers(C, max_denom, small_numbers);
    
    vec<MatQ> small_matrices = gen_matrices(small_numbers, 2);
    
    CliqueQ clique;
    generate_clique_for_2x2(clique, small_matrices, small_matrices.size());
    
    //result:

    // Matrix A1 = {{{5, 3}, {5, 3}}, 
    //              {{5, 3}, {4, 3}}};
    
    // Matrix A2 = {{{5, 3}, {5, 3}}, 
    //              {{5, 3}, {2, 1}}};
    
    // Matrix A3 = {{{-5, 3}, {5, 3}}, 
    //              {{-5, 3}, {4, 3}}};
    
    // Matrix A4 = {{{-5, 3}, {-5, 3}}, 
    //              {{5, 3}, {-4, 3}}};

    // Matrix X1 = {{{-5, 3}, {-5, 3}}, 
    //              {{-13, 3}, {-4, 3}}};
    
    // Matrix X2 = {{{-5, 3}, {1, 1}}, 
    //              {{-5, 3}, {4, 3}}};
}
