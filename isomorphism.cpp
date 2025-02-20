#include "lib/matrix-tools.h"
using namespace std;

int main() {
    
    vec<MatQ> Akbari = {
        MatQ({{Q(1, 1), Q(0, 1)},
            {Q(0, 1), Q(1, 1)}}),
        
        MatQ({{Q(0, 1), Q(1, 1)},
             {Q(1, 1), Q(0, 1)}}),
        
        MatQ({{Q(0, 1), Q(-1, 1)},
             {Q(-1, 1), Q(0, 1)}}),

        MatQ({{Q(0, 1), Q(1, 1)},
              {Q(-1, 1), Q(-2, 1)}}),
        
        MatQ({{Q(0, 1), Q(-1, 1)},
              {Q(1, 1), Q(-2, 1)}})
    };

    vec<MatQ> new_sample = {
        MatQ({{Q(2, 1), Q(2, 1)},
             {Q(2, 1), Q(1, 1)}}),
        
        MatQ({{Q(2, 1), Q(2, 1)},
             {Q(2, 1), Q(3, 1)}}),
        
        MatQ({{Q(-2, 1), Q(2, 1)},
              {Q(-2, 1), Q(1, 1)}}),

        MatQ({{Q(-2, 1), Q(-2, 1)},
              {Q(2, 1), Q(-1, 1)}}),
        
        MatQ({{Q(-2, 1), Q(-2, 1)},
              {Q(-4, 1), Q(-1, 1)}})
    };

    if (isomorphic_subgraphs_2_by_2(Akbari, new_sample)) 
        cout << "isomorphic subgraphs" << endl;
    else
        cout << "not isomorphic subgraphs" << endl;
    
}
