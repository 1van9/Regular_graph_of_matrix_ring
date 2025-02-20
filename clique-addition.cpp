#include <algorithm>
#include <random>
#include "lib/matrix-tools.h"
#include <bitset>

using namespace std;

inline MatRf gen_matrix_from_list(mt19937 & gen, const vec<MatRf> & all) {
    return all[gen() % all.size()];
}

vector<bool> was_add(6);
mt19937 gen(4385);
int cnt = 3;
const int sm_mat_sz = 2112;

Rf to_rf(const Poly<int> & x) {
    vec<Q> coefs;
    for (auto a : x.coefficients)
        coefs.emplace_back(a);
    Rf y(Poly<Q>(coefs), Poly<Q>(Q(1)));
    return y;
}

MatRf to_rf(const Matrix<Poly<int>> & x) {
    MatRf y(x.n, x.m);
    for (int i = 0; i < x.n; i++)
        for (int j = 0; j < x.m; j++)
            y[i][j] = to_rf(x[i][j]);
    return y;
}

void generate_clique_for_2x2(vec<int> & clique, const vec<Matrix<Poly<int>>> & small_matrices, int last_index, 
                             const vec<bitset<sm_mat_sz>> & g, const bitset<sm_mat_sz> & neighbours) {
    if (!cnt)
        return;
    if (clique.size() < 4) {
        while (last_index--) {
            if (neighbours[last_index]) {
                clique.push_back(last_index);
                generate_clique_for_2x2(clique, small_matrices, last_index, g, neighbours & g[last_index]);
                clique.pop_back();
            }
        }
    } else if (clique.size() == 4) {
        cout << "x"; cout.flush();
        vec<MatRf> cl;
        for (auto i : clique)
            cl.push_back(to_rf(small_matrices[i]));
        
        vec<MatRf> add = is_additionable_K4(cl);
        for (auto el : add) {
            cl.push_back(el);
            int add = expand(cl);
            cout << "find! addition = " << add << endl;
            if (add == 1 && cnt--) {
                for (auto el : cl) {
                    cout << el << endl;
                }
                cout << "---------------" << endl;
            }
            if (add == 3 && cnt--) {
                for (auto el : cl) {
                    cout << el << endl;
                }
                cout << "---------------" << endl;
            }
            if (add == 5 && cnt--) {
                for (auto el : cl) {
                    cout << el << endl;
                }
                cout << "---------------" << endl;
            }
            cl.pop_back();
        }
    }
    return;
}



int main() {
    int N = 2;
    Poly<int> y({0, 1});
    vec<Poly<int>> small_numbers = {Poly<int>(0), Poly<int>(1), -Poly<int>(1), y, -y, y + y - Poly<int>(1), -y-y + Poly<int>(1)};
    // generate_small_numbers(3, 2, small_numbers);
    Matrix<Poly<int>> curr(2);
    vec<Matrix<Poly<int>>> small_matrices = gen_matrices(small_numbers, 2);

    vec<bitset<sm_mat_sz>> g(sm_mat_sz);
    int edges = 0;
    for (int i = 0; i < sm_mat_sz; i++) {
        for (int j = 0; j < sm_mat_sz; j++)
            if (small_matrices[i].Connect(small_matrices[j]))
                g[i][j] = 1, edges++;
        if (i % 100 == 0) {
            cout << "o";
        }
    }
    cout << "k" << endl;
    vec<int> clique;
    bitset<sm_mat_sz> nei;
    for (int i = 0; i < sm_mat_sz; i++)
        nei[i] = 1;
    generate_clique_for_2x2(clique, small_matrices, small_matrices.size(), g, nei);
}
