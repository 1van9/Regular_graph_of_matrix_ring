#include <algorithm>
#include "lib/matrix-tools.h"
#include <bitset>
#include <cassert>

using namespace std;

using feild = Frac<Integer>;
using mat = Matrix<feild>;

int N = 2; // matrixes N x N
int expnd = 5; // find clique: expand(clique) = expnd


// parameters of the program is:

int seed = 13111; // seed for random generator
mt19937 gen_m(seed);
bool random_search = false; //search for cliques by enumerating random matrices 

bool random_numbers = false; // cells of matrixes are from list of size_of_nums random numbers
bool all_small_nums = true; // cells of matrixes are from list of all numbers a/b, a in [MINN, MAXN], b in [MIND, MAXD];
int C = 5;
int size_of_nums = 50, MINN = -C, MAXN = C, MIND = 1, MAXD = 1; 
// if random_numbers = false ans all_small_nums = false, chose numbers from list small_numbers
vec<Frac<long long>> small_numbers = {
    Frac<long long>(-2, 3), 
    Frac<long long>(2, 3), 
    Frac<long long>(-1, 3), 
    Frac<long long>(1, 3), 
    Frac<long long>(3), 
    Frac<long long>(-3), 
    Frac<long long>(1, 2), 
    Frac<long long>(-1, 2), 
    Frac<long long>(-2), 
    Frac<long long>(-1), 
    Frac<long long>(0), 
    Frac<long long>(1), 
    Frac<long long>(2)
};

int max_type = 2, min_type = 0; // is E1 and E2 already in clique
Matrix<Frac<long long>> E1({{Frac<long long>(5), Frac<long long>(5)}, {Frac<long long>(3), Frac<long long>(5)}});
Matrix<Frac<long long>> E2({{Frac<long long>(5), Frac<long long>(1)}, {Frac<long long>(-3), Frac<long long>(-5)}}); 

bool findcl = false;
const int sm_mat_sz = 20000;
long long cnt = 0;

const vec<int> numder_of_ops = {100, 100, 20000, 20000}; // number of steps during random clique generation 


void random_cliques_search(vec<mat> & clique, const vec<feild> & nums, mt19937 & gen) {
    if (findcl)
        return;
    if (clique.size() < 4) {
        int operations = numder_of_ops[clique.size()];
        while (!findcl && operations--) {
            mat next = random_matrix(nums, 2, gen);
            bool nei = true;
            for (auto v : clique)
                if (!next.Connect(v))
                    nei = false;
            if (nei) {
                clique.push_back(next);
                random_cliques_search(clique, nums, gen);
                clique.pop_back();
            }
        }
        if (clique.size() == 1)
            cout << "o";
    } else if (clique.size() == 4) {
        vec<mat> add = is_additionable_K4(clique);
        cnt += add.size();
        for (auto el : add) {
            clique.push_back(el);
            cout << "ok";
            // print(cl);
            int add = expand(clique);
            // cout << "find! addition = " << add << endl;
            if (add == expnd) {
                findcl = true;
                cout << "Expand = " << expnd << endl;
                for (auto el : clique) {
                    cout << el << endl;
                }
            }
            clique.pop_back();
        }
    }
    return;
}


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
    if (random_search) {
        vector<feild> nums;
        for (int i = MIND; i <= MAXD; i++) {
            for (int j = MINN; j <= MAXN; j++) {
                nums.emplace_back(j, i);
            }
        }
        vec<mat> clique;
        cout << "start" << endl;
        random_cliques_search(clique, nums, gen_m);
        cout << endl << "finish with cnt = " << cnt << endl;
        return 0;
    }
    if (random_numbers) {
        small_numbers.clear();
        for (int i = 0; i < size_of_nums; i++) {
            int num = 0, denum = 0;
            while (!denum || count(small_numbers.begin(), small_numbers.end(), Frac<long long>(num, denum))) 
                num = gen_m() % (MAXN - MINN + 1) + MINN, denum = gen_m() % (MAXD - MIND + 1) + MIND;
            small_numbers.push_back(Frac<long long>(num, denum));
        }
        E1 = random_matrix(small_numbers, N, gen_m);
        do {
            E2 = random_matrix(small_numbers, N, gen_m);
        } while (!E2.Connect(E1));
        cout << "first\n" << E1 << endl << "second\n" << E2 << endl;
    }
    if (all_small_nums) {
        set<Frac<long long>> nums;
        for (int i = MIND; i <= MAXD; i++) {
            for (int j = MINN; j <= MAXN; j++) {
                nums.insert(Frac<long long>(j, i));
            }
        }
        small_numbers = vector<Frac<long long>>(nums.begin(), nums.end());
    }

    auto all_matrixes = gen_matrices(small_numbers, N);
    cout << all_matrixes.size() << endl;
    for (int type = max_type; type >= min_type && !findcl; type--) 
    if (type == 2) {
        vec<Matrix<Frac<long long>>> small_matrices;
        for (auto m : all_matrixes) {
            if (m.Connect(E1) && m.Connect(E2)) {
                small_matrices.push_back(m);
            }
        }
        int indexE1 = small_matrices.size();
        small_matrices.push_back(E1);
        int indexE2 = small_matrices.size();
        small_matrices.push_back(E2);
        cout << "Number of matrixes : " << small_matrices.size() << endl;
        vec<bitset<sm_mat_sz>> g(small_matrices.size());
        vec<Matrix<Frac<Integer>>> sm;
        for (int i = 0; i < small_matrices.size(); i++) {
            Matrix<Frac<Integer>> a(2, 2);
            for (int x = 0; x < 2; x++)
                for (int y = 0; y < 2; y++)
                    a[x][y].fi = Integer(small_matrices[i][x][y].fi),
                    a[x][y].se = Integer(small_matrices[i][x][y].se);
            sm.push_back(a);
        }
        for (int i = 0; i < small_matrices.size(); i++) {
            for (int j = i + 1; j < small_matrices.size(); j++) {
                if (small_matrices[i].Connect(small_matrices[j]))
                    g[i][j] = 1, g[j][i] = 1;
            }
        }
        vec<int> clique;
        bitset<sm_mat_sz> nei;
        for (int i = 0; i < small_matrices.size() - 2; i++)
            nei[i] = 1;
        clique.push_back(indexE1);
        clique.push_back(indexE2);
        cnt = 0;
        cout << "type 2 Start" << endl;
        generate_clique_for_2x2(clique, sm, indexE1, g, nei);
        if (!findcl)
            cout << "finish, number of cliques = " << cnt << endl;
    } else if (type == 1) {
        vec<Matrix<Frac<long long>>> small_matrices;
        for (auto m : all_matrixes) {
            if (m.Connect(E1)) {
                small_matrices.push_back(m);
            }
        }
        int indexE1 = small_matrices.size();
        small_matrices.push_back(E1);
        cout << "Number of matrixes : " << small_matrices.size() << endl;
        vec<bitset<sm_mat_sz>> g(small_matrices.size());
        vec<Matrix<Frac<Integer>>> sm;
        for (int i = 0; i < small_matrices.size(); i++) {
            Matrix<Frac<Integer>> a(2, 2);
            for (int x = 0; x < 2; x++)
                for (int y = 0; y < 2; y++)
                    a[x][y].fi = Integer(small_matrices[i][x][y].fi),
                    a[x][y].se = Integer(small_matrices[i][x][y].se);
            sm.push_back(a);
        }
        for (int i = 0; i < small_matrices.size(); i++) {
            for (int j = i + 1; j < small_matrices.size(); j++) {
                if (small_matrices[i].Connect(small_matrices[j]))
                    g[i][j] = 1, g[j][i] = 1;
            }
        }
        vec<int> clique;
        bitset<sm_mat_sz> nei;
        for (int i = 0; i < small_matrices.size() - 1; i++)
            nei[i] = 1;
        clique.push_back(indexE1);
        cnt = 0;
        cout << "type 1 Start" << endl;
        generate_clique_for_2x2(clique, sm, indexE1, g, nei);
        if (!findcl)
            cout << "finish, number of cliques = " << cnt << endl;
    } else {
        auto small_matrices = all_matrixes;
        int index = small_matrices.size();
        cout << "Number of matrixes : " << small_matrices.size() << endl;
        vec<bitset<sm_mat_sz>> g(small_matrices.size());
        int edges = 0;
        vec<Matrix<Frac<Integer>>> sm;
        for (int i = 0; i < small_matrices.size(); i++) {
            Matrix<Frac<Integer>> a(2, 2);
            for (int x = 0; x < 2; x++)
                for (int y = 0; y < 2; y++)
                    a[x][y].fi = Integer(small_matrices[i][x][y].fi),
                    a[x][y].se = Integer(small_matrices[i][x][y].se);
            sm.push_back(a);
        }
        for (int i = 0; i < small_matrices.size(); i++) {
            for (int j = i + 1; j < small_matrices.size(); j++) {
                if (small_matrices[i].Connect(small_matrices[j]))
                    g[i][j] = 1, g[j][i] = 1;
            }
        }
        vec<int> clique;
        bitset<sm_mat_sz> nei;
        for (int i = 0; i < small_matrices.size(); i++)
            nei[i] = 1;
        cnt = 0;
        cout << "type 0 Start" << endl;
        generate_clique_for_2x2(clique, sm, index, g, nei);
        if (!findcl)
            cout << "finish, number of cliques = " << cnt << endl;
    }
}
