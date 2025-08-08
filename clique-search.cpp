#include "lib/matrix-tools.h"

using namespace std;

using Feild = Frac<long long>;
using mat = Matrix<Feild>;

int best = 13;
vec<mat> ans = {
mat({{1, 1, -1},
    {1, 0, 0},
    {1, -1, 0}
}),
mat({{1, 1, 1},
    {-1, 0, 0},
    {1, -1, 0}
}),
mat({{-1, 1, -1},
    {-1, 0, 0},
    {1, -1, 0}
}),
mat({{1, 1, 1},
    {1, 0, 0},
    {1, 1, 0}
}),
mat({{-1, 1, 1},
    {1, 0, 0},
    {1, -1, 0}
}),
mat({{-1, 1, 1},
    {-1, 0, 0},
    {1, 1, 0}
}),
mat({{1, 1, -1},
    {-1, 0, 0},
    {1, 1, 0}
}),
mat({{-1, 1, -1},
    {1, 0, 0},
    {1, 1, 0}
})
};
vec<mat> all;

void GenerationAns(vec<mat>& curr_ans, int index) {
    if (curr_ans.size() > best) {
        best = curr_ans.size();
        for (int i = 0; i < curr_ans.size(); i++) {
            for (int j = i + 1; j < curr_ans.size(); j++) {
                assert(curr_ans[i].Connect(curr_ans[j]));
            }
        }
        std::cout << best << std::endl;
        print(curr_ans);
    }
    if (index == all.size())
        return;
    GenerationAns(curr_ans, index + 1);
    mat nw = all[index];
    bool check = true;
    for (auto m : curr_ans)
        check &= nw.Connect(m);
    if (check) {
        curr_ans.push_back(nw);
        GenerationAns(curr_ans, index + 1);
        curr_ans.pop_back();
    }
}


int main() {
    for (auto m1 : gen_matrices<Feild>({-1, 0, 1}, 3)) {
        bool con = true;
        for (auto m2 : ans)
            con &= m1.Connect(m2);
        if (con)
            all.push_back(m1);
    }
    // all = gen_matrices<long long>({-1, 0, 1}, 3);
    std::cout << all.size() << std::endl;
    GenerationAns(ans, 0);
    return 0;
}
