//
//  main.cpp
//  3_LCA
//
//  Created by Ivan on 15.05.17.
//  Copyright © 2017 Ivan. All rights reserved.
//

#include <iostream>
#include "vector"

struct CNode{
    int num;
    CNode* parent;
    CNode(int _num, CNode* _parent) : num(_num), parent(_parent) {};
    CNode(int _num) : num(_num), parent() {};
    ~CNode();
    std::vector<CNode*> children;
};

bool is_first_parent_second(int first, int second, std::vector<std::pair<int, int> >& time){
    if(time[first].first < time[second].first && time[second].second < time[first].second){
        return true;
    }
    return false;
}

//запрос
int Ask(int a1, int a2, std::vector<CNode*>& tree, std::vector<std::vector<int>>& P, std::vector<std::pair<int, int> >& time, std::vector<int>& logarifm){
    if(a1 == a2){
        return a1;
    }
    if(is_first_parent_second(a1, a2, time)){
        return a1;
    }
    if(is_first_parent_second(a2, a1, time)){
        return a2;
    }
    
    int L = logarifm[logarifm.size() - 1];
    while (L >= 0) {
        if(!is_first_parent_second(P[a1][L], a2, time)){
            a1 = P[a1][L];
        }
        --L;
    }
    return P[a1][0];
}

//быстрый подсчёт логарифма
void fastLog(std::vector<int>& logarifm){
    int j = 1;
    for(int i = 1; i < logarifm.size() + 1; ++i){
        if(i < j){
            logarifm[i] = logarifm[i - 1];
        }
        else{
            logarifm[i] = logarifm[i - 1] + 1;
            j *= 2;
        }
    }
}

//подсчёт времени входа и выхода
void dfs(int& t, CNode* curr, std::vector<CNode*>& tree, std::vector<std::pair<int, int> >& time){
    time[curr->num].first = t;
    ++t;
    for(int i = 0; i < curr->children.size(); ++i){
        dfs(t, curr->children[i], tree, time);
        ++t;
    }
    time[curr->num].second = t;
}

void make_parent(std::vector<CNode*>& tree, std::vector<int>& logarifm, std::vector<std::vector<int> >& P){
    int n = P.size();
    for(int i = 0; i < n; ++i){
        P[i].resize(logarifm[n - 1] + 1, 0);
        if(tree[i]->parent != NULL){
            P[i][0] = tree[i]->parent->num;
        }
    }
    
    for(int j = 1; j < logarifm[n]; ++j){
        for(int i = 1; i < n; ++i){
            if(tree[P[P[i][j - 1]][j - 1]]->parent != NULL){
                P[i][j] = P[P[i][j - 1]][j - 1];
            }
        }
    }
}

int main(int argc, const char * argv[]) {
    int n = 0, m = 0;
    std::cin >> n >> m;
    int curr_parent = 0;
    std::vector<CNode*> tree;
    CNode* root = new CNode(0, NULL);
    
    //coздаём дерево
    for(int i = 0; i < n; ++i){
        CNode* curr_node = new CNode(i, NULL);
        tree.push_back(curr_node);
    }
    tree[0] = root;
    //заполняем дерево
    for(int i = 1; i < n; ++i){
        std::cin >> curr_parent;
        tree[i]->parent = tree[curr_parent];
        tree[curr_parent]->children.push_back(tree[i]);
    }
    
    //создаём и заполняем массив логарифма
    std::vector<int> logarifm(n + 1, 0);
    fastLog(logarifm);
    
    //создаём и заполняем массив 2**i предков
    std::vector<std::vector<int>> P(n);
    make_parent(tree, logarifm, P);
    
    //предподсчитываем время входа и выхода для каждой вершины
    std::vector<std::pair<int, int> > time(n);
    int t = 0;
    dfs(t, root, tree, time);
    
    //ввод данных
    long long a1 = 0, a2 = 0;
    std::cin >> a1 >> a2;
    
    long long x = 0, y = 0, z = 0;
    std::cin >> x >> y >> z;
    
    long long result = 0;
    long long ans = 0;
    long long first = 0, second = 0;
    for(int i = 0; i < m; ++i){
        first = (a1 + result) % n;
        second = a2;
        result = Ask(first, second, tree, P, time, logarifm);

        a1 = (x * a1 + y * a2 + z) % n;
        a2 = (x * a2 + y * a1 + z) % n;
        ans += result;
    }
    
    std::cout << ans;
    return 0;
}
