#include "lib/matrix-tools.h"
#include <map>
#include <unordered_map>

using namespace std;

const int prime = 5;
const int N = (prime * prime - 1) * (prime * prime - prime);

using feild = Z<prime>;
using mat = Matrix<feild>;

bool connection[N][N];
vec<mat> all;
map<mat, int> indexes;
int cnt = 0;

// set<vec<int>> all_perms;

vec<bool> to1[N], to2[N];
// int deg[N];
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

void go(int st = 0) {
    if (cnt)
        return;
    if (already == N) {
        map<int, int> mp;
        bool unexpected_with_neg = false;
        for (int i = 0; i < N; i++) {
            result_perm.push_back(perm[i]);
            mp[i] = perm[i];
        }
        if (!is_standard_automorphism(all, mp)) {
            cout << "find" << endl;
            cnt++;
        } else {
            result_perm.clear();
        }
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
            // cout << already << endl;
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


bool check() {
    map<int, int> A;
    if (*max_element(result_perm.begin(), result_perm.end()) + 1 != result_perm.size()) {
        cout << "BAD not permutation" << endl;
        return false;
    }
    if (*min_element(result_perm.begin(), result_perm.end()) != 0) {
        cout << "BAD not permutation" << endl;
        return false;
    }
    if (set(result_perm.begin(), result_perm.end()).size() != result_perm.size()) {
        cout << "BAD not permutation" << endl;
        return false;
    }
    for (int i = 0; i < N; i++) {
        A[i] = result_perm[i];
        for (int j = i + 1; j < N; j++) {
            if (connection[i][j] != connection[result_perm[i]][result_perm[j]]) {
                cout << "BAD not automorphism" << endl;
                return false;
            }
        }
    }
    bool fl = is_standard_automorphism(all, A);
    bool clear_check = false;
    for (int i = 0; i < N && clear_check; i++) {
        for (int j = 0; j < N && clear_check; j++) {
            bool ok1 = true, ok2 = true;
            for (int t = 0; t < N; t++) {
                if (all[i] * all[t] * all[j] != all[result_perm[t]]) ok1 = false;
                if (all[i] * all[t].t() * all[j] != all[result_perm[t]]) ok2 = false;
            }
            if (ok1 || ok2) {
                clear_check = true;
            }
        }
    }
    if (clear_check != fl) {
        cout << "BAD standart automorphis checking" << endl;
        return false;
    }
    if (!fl) {
        return true;
    }
    cout << "BAD standart aut" << endl;
    return false;
}


bool find_bad_aut_with_stable_akb() {
    cout << "Checking automorphism with stable Akbari clique" << endl;
    vec<mat> akb = samples::Akbari<feild>();
    vec<int> p(prime);
    for (int i = 0; i < prime; i++) p[i] = i;
    // cout << "Finish prep" << endl;
    for (int i = 0; i < N; i++)
        perm[i] = -1;
    for (int i = 0; i < akb.size(); i++)
        add(indexes[akb[i]], indexes[akb[i]]);
    go();
    if (cnt) {
        if (!check()) {
            cout << "ERROR" << endl;    
        }
        cout << "find bad automorphism!" << endl;
        return true;
    }
    cnt = 0;
    for (int i = 0; i < akb.size(); i++)
        pop(indexes[akb[i]], indexes[akb[i]]);
    cout << "All automorphism is standart" << endl;
    return false;
}


int main() {
    vec<feild> a = {feild(0), feild(1), feild(2), feild(3), feild(4)};
    all = gen_matrices<feild>(a, 2);
    assert(N == all.size());
    cout << "number of matrices " << N << endl;
    for (int i = 0; i < N; i++) {
        indexes[all[i]] = i;
        for (int j = 0; j < N; j++)
            if (all[i].Connect(all[j]))
                connection[i][j] = true;
    }
    cout << find_bad_aut_with_stable_akb() << endl;

    int eq = 0;
    for (int i = 0; i < N; i++) {
        if (result_perm[i] == i)
            eq++;
    }
    cout << "Number of stable dots = " << eq << "/" << N << endl;
    // freopen("example5.txt", "w", stdout);
    // for (int i = 0; i < N; i++) {
    //     cout << all[i][0][0] << " " << all[i][0][1] << "   ->   " << all[result_perm[i]][0][0] << " " << all[result_perm[i]][0][1] << endl;  
    //     cout << all[i][1][0] << " " << all[i][1][1] << "        " << all[result_perm[i]][1][0] << " " << all[result_perm[i]][1][1] << endl;
    //     cout << endl;  
    // }
    // freopen("stable.txt", "w", stdout);
    // set<mat> stable;
    // for (int i = 0; i < N; i++) {
    //     if (result_perm[i] == i)
    //         // cout << all[i] << endl;
    //         stable.insert(all[i]);    
    // }
    // bool fl = true;
    // mat E = mat({{1, 0}, {0, 1}});
    // for (auto x : stable) {
    //     if (!count(stable.begin(), stable.end(), x.rev())) {
    //         cout << x << " has no inverse" << endl;
    //             fl = false;
    //     }
    // }
    bool fl = true;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int i1 = result_perm[i], j1 = result_perm[j];
            bool conn1 = ((all[i] - all[j]).det() == 0);
            bool conn2 = ((all[i1] - all[j1]).det() == 0);
            if (conn1 != conn2)
                fl = false;
        }
    }
    if (fl) 
        cout << "save difference graph" << endl;
    else
        cout << "Not save difference graph" << endl;
    fl = true;
    for (int i = 0; i < N && fl; i++) {
        int j = result_perm[i];
        int i_inv = indexes[all[i].rev()], j_inv = indexes[all[j].rev()];
        if (result_perm[i_inv] != j_inv) {
            fl = false;
        }
    }
    if (fl) {
        cout << "save inverse" << endl;
    } else {
        cout << "Not save inverse" << endl;
    }
    fl = true;
    for (int i = 0; i < N && fl; i++) {
        int j = result_perm[i];
        int i1 = indexes[all[i] + all[i]], j1 = indexes[-all[j] - all[j]];
        if (result_perm[i1] != j1) {
            fl = false;
        }
    }
    if (fl) {
        cout << "save 2 property" << endl;
    } else {
        cout << "Not save 2 property" << endl;
    }
    fl = true;
    for (int i = 0; i < N && fl; i++) {
        int j = result_perm[i];
        int i1 = indexes[all[i].t()], j1 = indexes[all[j].t()];
        if (result_perm[i1] != j1) {
            fl = false;
        }
    }
    if (fl) {
        cout << "save transposal" << endl;
    } else {
        cout << "Not save transposal" << endl;
    }
    fl = true;
    for (int i = 0; i < N && fl; i++) {
        for (int j = 0; j < N && fl; j++) {
            int i1 = result_perm[i], j1 = result_perm[j];
            if (result_perm[indexes[all[i] + all[j]]] != indexes[-all[i1] - all[j1]]) {
                fl = false;
            }
        }
    }
    if (fl) {
        cout << "save -additivity" << endl;
    } else {
        cout << "Not save -additivity" << endl;
    }
    return 0;
}
