#include "lib/matrix-tools.h"

int best = 13;
vec<Mat> ans = {
Mat({{1, 1, -1},
    {1, 0, 0},
    {1, -1, 0}
}),
Mat({{1, 1, 1},
    {-1, 0, 0},
    {1, -1, 0}
}),
Mat({{-1, 1, -1},
    {-1, 0, 0},
    {1, -1, 0}
}),
Mat({{1, 1, 1},
    {1, 0, 0},
    {1, 1, 0}
}),
Mat({{-1, 1, 1},
    {1, 0, 0},
    {1, -1, 0}
}),
Mat({{-1, 1, 1},
    {-1, 0, 0},
    {1, 1, 0}
}),
Mat({{1, 1, -1},
    {-1, 0, 0},
    {1, 1, 0}
}),
Mat({{-1, 1, -1},
    {1, 0, 0},
    {1, 1, 0}
})
};

vec<Mat> all;

void GenerationAns(vec<Mat>& curr_ans) {
    if (curr_ans.size() > best) {
        best = curr_ans.size();
        std::cout << best << std::endl;
        print(curr_ans);
    }
    int cnt_try = 1000;
    int maxc = 1;
    if (curr_ans.size() >= 10) {
        cnt_try = 5000;
    }
    while (cnt_try--) {
        Mat nw = all[rand() % all.size()];
        bool check = true;
        for (auto& a : curr_ans) {
            if (!a.Connect(nw)) {
                check = false;
                break;
            }
        }
        if (check) {
            curr_ans.push_back(nw);
            GenerationAns(curr_ans);
            curr_ans.pop_back();
        }
    }
}


int main() {
    all = gen_matrices<long long>({-1, 0, 1}, 3);
    // std::cout << all.size() << std::endl;
    GenerationAns(ans);
    return 0;
}