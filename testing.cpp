//
// Created by alonb on 04/01/2022.
//

#include <iostream>
#include "UnionFind.h"


using namespace union_find;
using namespace std;

int pow(int n) {
    if (n == 0) return 1;
    if (n < 0) throw std::exception();
    if (n > 0) return 2 * pow(n - 1);
}

static const int N = pow(5);


int main() {

    UnionFind UF = UnionFind(N);
    for (int i = 1; i <= N; ++i) {
        UF.makeSet(i, i);
    }

    for(int i = 1; i < N; i*=2){
        for (int j = i; j < 2 * i; ++j) {
            UF.unionSet(j, j+1);
        }
    }

    cout << UF;

    return 0;
}
