//
//  main.cpp
//  1_second_statistic
//
//  Created by Ivan on 08.05.17.
//  Copyright © 2017 Ivan. All rights reserved.
//

#include <iostream>
#include <vector>
#include "math.h"
#include "set"

// структура хранящая два минимума для более удобного пользования
struct CStructTwoMin{
    int first, second;
    CStructTwoMin(): first(1000000000), second(1000000000) {}
    CStructTwoMin(int first_, int second_): first(first_), second(second_) {}

};

//функция слияния двух элементов таблицы при переходе на следующий уровень
CStructTwoMin CMin(CStructTwoMin& self, CStructTwoMin& other){
    CStructTwoMin ans = CStructTwoMin();
    std::set<int> S;
    
    S.insert(self.first);
    S.insert(other.first);
    S.insert(self.second);
    S.insert(other.second);
    
    ans.first = *S.begin();
    ans.second = *(++S.begin());
    
    return ans;
}


void MakeSparseTable(std::vector<int>& A, std::vector<std::vector<CStructTwoMin> >& SparseTable, std::vector<int>& logarifm){
    int INF = INT_MAX;
    int n = A.size();
    
    //создание и заполнение первого слоя
    for(int i = 0; i < n; ++i){
        CStructTwoMin curr = CStructTwoMin(A[i], INF);
        SparseTable[0].push_back(curr);
    }
    
    // создание таблицы
    int len = n;
    CStructTwoMin curr = CStructTwoMin();
    for(int i = 1; i < logarifm[n - 1] + 1; ++i){
        len -= pow(2, i - 1);
        for(int j = 0; j < len; ++j){
            SparseTable[i].push_back(curr);
            SparseTable[i][j] = CMin(SparseTable[i - 1][j], SparseTable[i - 1][j + pow(2, i - 1)]);
        }
    }
}

int RMQ(int from, int to, std::vector<std::vector<CStructTwoMin> >& SparseTable, std::vector<int>& logarifm){
    int k = logarifm[to - from + 1];
    return CMin(SparseTable[k][from], SparseTable[k][to - pow(2, k) + 1]).second;
}

void fastLog(std::vector<int>& logarifm){
    int j = 1;
    for(int i = 1; i < logarifm.size(); ++i){
        if(i < j){
            logarifm[i] = logarifm[i - 1];
        }
        else{
            logarifm[i] = logarifm[i - 1] + 1;
            j *= 2;
        }
    }

}


int main(int argc, const char * argv[]) {
    int n = 0, b = 0;
    std::cin >> n >> b;
    std::vector<int> A;
    int curr = 0;
    for(int i = 0; i < n; ++i){
        std::cin >> curr;
        A.push_back(curr);
    }
    
    std::vector<int> logarifm(n + 1, -1);
    fastLog(logarifm);
    
    std::vector<std::vector<CStructTwoMin> > SparseTable(logarifm[n - 1] + 1);
    MakeSparseTable(A, SparseTable, logarifm);
    
    int from = 0, to = 0;
    for(int i = 0; i < b; ++i){
        std::cin >> from >> to;
        std::cout << RMQ(from - 1, to - 1, SparseTable, logarifm) << '\n';
    }
    
    return 0;
}
