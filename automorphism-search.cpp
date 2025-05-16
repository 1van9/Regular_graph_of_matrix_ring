#include "lib/matrix-tools.h"
#include "lib/basic_types/Z4.h"
#include <map>
#include <unordered_map>

using namespace std;

const int prime = 4; // 3, 5, 7
const int N = (prime * prime - 1) * (prime * prime - prime);

using field = Z4; // Z<prime>
using mat = Matrix<field>;

bool connection[N][N];
vec<mat> all;
map<mat, int> indexes;
int cnt = 0;


vec<bool> to1[N], to2[N];
int max_deg = -1;

int already = 0;
int perm[N];
bool used[N];

void add(int i, int j) {
    perm[i] = j;
    used[j] = true;
    already++;
    for (int k = 0; k < N; k++) {
        to1[k].push_back(connection[k][i]), to2[k].push_back(connection[k][j]);
    }
}

void pop(int i, int j) {
    for (int k = 0; k < N; k++)
        to1[k].pop_back(), to2[k].pop_back();
    already--;
    used[j] = false;
    perm[i] = -1;
}

vec<int> result_perm;
vec<vec<int>> all_aut;

mat varphi4(mat A) {
    int phi[4] = {0, 1, 3, 2};
    mat B(A.n, A.m);
    for (int i = 0; i < A.n; i++) {
        for (int j = 0; j < A.m; j++) {
            B[i][j] = Z4(phi[A[i][j].x]);
        }
    }
    return B;
}

void go() {
    if (already == N) {
        map<int, int> mp1, mp2;
        for (int i = 0; i < N; i++) {
            result_perm.push_back(perm[i]);
            mp1[i] = perm[i];
            mp2[i] = indexes[varphi4(all[perm[i]])]; // only for Z4
        }
        all_aut.push_back(result_perm);
        if (!is_standard_automorphism(all, mp1) && !is_standard_automorphism(all, mp2)) { // only for Z4
            cout << "BAD automorphism was detected" << endl;
            for (int i = 0; i < N; i++) {
                cout << i << " " << result_perm[i] << endl;
            }
            exit(0);
        }
        cnt++;
        result_perm.clear();
        return;
    }

    unordered_map<vec<bool>, vec<int>> mp1, mp2;
    for (int i = 0; i < N; i++) {
        if (!used[i]) {
            mp2[to2[i]].push_back(i);
        }
    }

    for (int i = 0; i < N; i++) {
        if (perm[i] == -1) {
            mp1[to1[i]].push_back(i);
        }
    }
    int i = -1, sm = N + 1;
    for (auto [x, y] : mp1) {
        if (y.size() != mp2[x].size()) {
            return;
        }
        if (y.size() < sm) {
            i = y[0];
        }
    }
    for (int j : mp2[to1[i]]) {
        add(i, j);
        go();
        pop(i, j);
    }
}


void find_all_aut() {
    mat E = mat({{field(1), field(0)}, {field(0), field(1)}});
    for (int i = 0; i < N; i++)
        perm[i] = -1;
    add(indexes[E], indexes[E]);
    go();
    pop(indexes[E], indexes[E]);
}

int main() {
    vec<field> nums;
    for (int i = 0; i < prime; i++)
        nums.emplace_back(i);
    all = gen_matrices<field>(nums, 2);
    assert(N == all.size());
    cout << "Number of matrices: " << N << endl;
    
    for (int i = 0; i < N; i++) {
        indexes[all[i]] = i;
        for (int j = 0; j < N; j++)
            if (all[i].Connect(all[j]))
                connection[i][j] = true;
    }
    find_all_aut(); // E -> E
    cout << "All " << cnt <<  " automorphisms are standard" << endl;
    
    map<int, int> I;
    for (int i = 0; i < N; i++)
        I[i] = indexes[all[i].rev()];
    cout << "I is standard in Regular G - " << boolalpha << is_standard_automorphism(all, I) << endl;
    cout << "I is standard in Total G - " << boolalpha << is_standard_automorphism(all, I, false) << endl;
    return 0;
}
