#include <algorithm>
#include "lib/matrix-tools.h"
#include <bitset>

using namespace std;

const int prime = 19; // 47, 41, 29, 23, 17, 13, 11, 7, 5 -- no examples
// 43, 37, 31, 19 - expand 3 exists

using feild = Z<prime>;
using mat = Matrix<feild>;

int expnd = 3;
int N = 2;
bool findcl = false;
const int sm_mat_sz = 10000;
long long cnt = 0;
mat E = mat({{feild(1), feild(0)}, {feild(0), feild(1)}});

void generate_clique_for_2x2(vec<int> & clique, const vec<mat> & small_matrices, int last_index, 
                             const vec<bitset<sm_mat_sz>> & g, const bitset<sm_mat_sz> & neighbours) {
    if (findcl)
        return;
    if (clique.size() < 4) {
        while (!findcl && last_index--) {
            if (neighbours[last_index]) {
                clique.push_back(last_index);
                generate_clique_for_2x2(clique, small_matrices, last_index, g, neighbours & g[last_index]);
                clique.pop_back();
            }
        }
    } else if (clique.size() == 4) {
        vec<mat> cl;
        for (auto i : clique)
            cl.push_back(small_matrices[i]);
        
        vec<mat> add = is_additionable_K4(cl);
        cnt += add.size();
        for (auto el : add) {
            cl.push_back(el);
            int add = expand(cl);
            if (add == expnd) {
                findcl = true;
                cout << "Expand = " << expnd << endl;
                for (auto el : cl) {
                    cout << el << endl;
                }
                cout << "Expand with matrixes :" << endl;
                for (int j = 0; j < 5; j++) {
                    vec<mat> all = cl;
                    all.erase(all.begin() + j);
                    vec<mat> nw = is_additionable_K4(all);
                    if (nw.size() == 2) {
                        if (find(cl.begin(), cl.end(), nw[0]) != cl.end()) {
                            cout << nw[1] << endl;
                        } else {
                            cout << nw[0] << endl;
                        }
                    }
                }            
            }
            cl.pop_back();
        }
    }
    return;
}


int main() {
    vector<feild> nums;
    for (int i = 0; i < prime; i++)
        nums.emplace_back(i);

    auto all_matrixes = gen_matrices(nums, N);
    vec<mat> E2s;
    for (int l = 1; l < prime; l++)
        if ((feild(1)/feild(l)).x >= feild(l).x)
            E2s.push_back(mat({{feild(-1), feild(0)}, {feild(0), feild(l)}}));
    E2s.push_back(mat({{feild(-1), feild(1)}, {feild(0), feild(-1)}}));

    for (mat E2 : E2s) {
        if (findcl)
            break;
        vec<mat> small_matrices;
        for (auto m : all_matrixes) {
            if (m.Connect(E) && m.Connect(E2))
                small_matrices.push_back(m);
        }
        int index1 = small_matrices.size();
        small_matrices.push_back(E);
        int index2 = small_matrices.size();
        small_matrices.push_back(E2);
        cout << "Number of matrixes : " << small_matrices.size() << endl;
        vec<bitset<sm_mat_sz>> g(small_matrices.size());
        for (int i = 0; i < small_matrices.size(); i++) {
            for (int j = i + 1; j < small_matrices.size(); j++) {
                if (small_matrices[i].Connect(small_matrices[j]))
                    g[i][j] = 1, g[j][i] = 1;
            }
        }
        vec<int> clique;
        clique.push_back(index1);
        clique.push_back(index2);
        bitset<sm_mat_sz> nei;
        for (int i = 0; i < small_matrices.size() - 2; i++)
            nei[i] = 1;
        cnt = 0;
        cout << "start" << endl;
        generate_clique_for_2x2(clique, small_matrices, index1, g, nei);
        cout << "finish, number of cliques = " << cnt << endl;
    }
    return 0;
}
