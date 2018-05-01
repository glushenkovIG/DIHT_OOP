//
//  main.cpp
//  4_Hemming
//
//  Created by Ivan on 27.04.17.
//  Copyright © 2017 Ivan. All rights reserved.
//

#include <iostream>
#include <vector>
#include <queue>
#include <fstream>

class IGraph {
public:
    virtual ~IGraph() {}
    
    virtual void AddEdge(int from, int to, int weight) = 0;
    
    virtual int VerticesCount() const = 0;
    
    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const = 0;
    
    virtual int GetWeight(int from, int to) const = 0;
};

class MatrixGraph: public IGraph {
public:
    MatrixGraph(int n);
    ~MatrixGraph(){}
    
    void AddEdge(int from, int to, int weight) override;
    
    int VerticesCount() const override;
    
    void GetNextVertices(int vertex, std::vector<int>& vertices) const override;
    
    int GetWeight(int from, int to) const override;
    
private:
    std::vector<std::vector<int>> Matrix;
};

MatrixGraph::MatrixGraph(int count) :
Matrix(count)
{
    for(int i = 0; i < count; ++i){
        Matrix[i].clear();
        Matrix[i].resize(count, 0);
    }
}

int MatrixGraph::VerticesCount() const {
    return int(Matrix.size());
}

void MatrixGraph::AddEdge(int from, int to, int weight) {
    Matrix[from][to] = weight;
}

int MatrixGraph::GetWeight(int from, int to) const{
    return Matrix[from][to];
}

void MatrixGraph::GetNextVertices(int from, std::vector<int>& vertices) const{
    for(int i = 0; i < Matrix.size(); ++i){
        if(Matrix[from][i] != 0){
            vertices.push_back(i);
        }
    }
}

//проверяем есть ли вообще путь из from в to
bool bfs(int from, int to, int n, std::vector<int>& level, const IGraph& Capacity, const IGraph& CurrFlow) {
    level.clear();
    level.resize(n, -1);
    level[from] = 0;
    std::queue<int> q;
    q.push(from);
    while (!q.empty()) {
        int curr = q.front();
        q.pop();
        for (int next = 0; next < n; ++next) {
            if (level[next] == -1 && Capacity.GetWeight(curr, next) > CurrFlow.GetWeight(curr, next)) {
                q.push(next);
                level[next] = level[curr] + 1;
            }
        }
    }
    return level[to] != -1;
}

//  ищем d_flow - на сколько можно увеличить данный поток flow
//    (это будет блокирующий поток для данной слоистой сети)
//      (её я отдельно не строю так как достаточно просто проверять чтоб разность уровней у вершин была 1
//         это мы в бфс получили когда проверяли существование пути как такового)
int dfs(int curr, int d_flow, int from, int to, int n, std::vector<int>& level, std::vector<int>& lastDelVert, const IGraph& Capacity, IGraph& CurrFlow) {
    if (curr == to){
        return d_flow;
    }
    if (d_flow == 0){
        return 0;
    }
    for (int& next = lastDelVert[curr]; next < n; ++next) {
        if (level[next] == level[curr] + 1){
            int d_edge_flow = dfs(next, std::min(d_flow, Capacity.GetWeight(curr, next) - CurrFlow.GetWeight(curr, next)), from, to, n, level, lastDelVert, Capacity, CurrFlow);
            if (d_edge_flow != 0) {
                CurrFlow.AddEdge(curr, next, CurrFlow.GetWeight(curr, next) + d_edge_flow);
                CurrFlow.AddEdge(next, curr, CurrFlow.GetWeight(next, curr) - d_edge_flow);
                return d_edge_flow;
            }
        }
    }
    return 0;
}

void getMaxFlow(int from, int to, const IGraph& Capacity, IGraph& CurrFlow) {
    int max_flow = 0;
    int n = Capacity.VerticesCount();
    std::vector<int> level(n, -1);// список уровней для слоистой сети
    std::vector<int> lastDelVert(n, 0); // массив для хранения последней удалённой вершины при поиске блок потока
    
    //пока существует путь то значит можно пустить поток какого то ненулевого веса
    while(bfs(from, to, n, level, Capacity, CurrFlow)) {
        lastDelVert.clear();
        lastDelVert.resize(n, 0);
        
        while (int d_flow = dfs(from, 100000000, from, to, n, level, lastDelVert, Capacity, CurrFlow)){
            max_flow += d_flow;
        }
    }
    // пути больше нет значит мы нашли максимальный поток
}

void find_achievable_vertex(int curr, const IGraph& Capacity, const IGraph& CurrFlow, std::vector<int>& visited){
    visited[curr] = 1;
    std::vector<int> A;
    Capacity.GetNextVertices(curr, A);
    for(int& elem : A){
        if(visited[elem] == 0 && Capacity.GetWeight(curr, elem) > CurrFlow.GetWeight(curr, elem)){
            find_achievable_vertex(elem, Capacity, CurrFlow, visited);
        }
    }
    return;
}

int main(int argc, const char * argv[]) {
    std::string s;
    std::string p;
    
    std::cin >> s;// считаем строку
    std::cin >> p;// считаем шаблон
    
    int s_size = s.size();
    int p_size = p.size();
    
    std::string A = s + p;
    std::vector<int> num(s_size + p_size);// вектор с нумерованными знаками вопрос
    int ask_count = 0;
    for(int i = 0; i < s_size + p_size; ++i){
        if (A[i] == '?'){
            num[i] = ++ask_count;
        }
    }
    MatrixGraph Capacity(ask_count + 2);
    //создадим граф
    for(int i = 0; i <= s_size - p_size; ++i){
        int d = i;//нумерация в s с учётом сдвига
        int j = 0;
        while(d < s_size && j < p_size){
            if(p[j] == '?' && s[d] == '?'){
                int old = Capacity.GetWeight(num[j + s_size], num[d]);
                Capacity.AddEdge(num[j + s_size], num[d], old + 1);
                Capacity.AddEdge(num[d], num[j + s_size],  old + 1);
            }
            if (p[j] == '?' && s[d] == '0'){
                int old = Capacity.GetWeight(num[j + s_size], 0);
                Capacity.AddEdge(num[j + s_size], 0, old + 1);
                Capacity.AddEdge(0, num[j + s_size], old + 1);

            }
            if (p[j] == '?' && s[d] == '1'){
                int old = Capacity.GetWeight(num[j + s_size], ask_count + 1);
                Capacity.AddEdge(num[j + s_size], ask_count + 1, old + 1);
                Capacity.AddEdge(ask_count + 1, num[j + s_size], old + 1);

            }
            if(p[j] == '0' && s[d] == '?'){
                int old = Capacity.GetWeight(0, num[d]);
                Capacity.AddEdge(0, num[d], old + 1);
                Capacity.AddEdge(num[d], 0, old + 1);

            }
            if(p[j] == '1' && s[d] == '?'){
                int old = Capacity.GetWeight(ask_count + 1, num[d]);
                Capacity.AddEdge(ask_count + 1, num[d], old + 1);
                Capacity.AddEdge(num[d], ask_count + 1, old + 1);

            }
            ++j;
            ++d;
        }
    }
    
    // граф текущего потока из from в to
    // в нем есть вершины 0 и 1 и так же все вопросы
    MatrixGraph CurrFlow = MatrixGraph(ask_count + 2);
    
    //получим максимальный поток из 0 в 1
    getMaxFlow(0, ask_count + 1, Capacity, CurrFlow);
    
    // найдём все вопросы которые достижимы из нуля в остаточной сети
    std::vector<int> visited(ask_count + 2);
    find_achievable_vertex(0, Capacity, CurrFlow, visited);
    
    //теперь все что достижимы заменим на ноль а остальные на единицу
    int j = 0;
    for(int i = 0; i < s_size + p_size; ++i){
        if(A[i] == '?' && visited[j + 1] == 1){
            A[i] = '0';
            ++j;
        }
        else{
            if(A[i] == '?' && visited[j + 1] == 0){
                A[i] = '1';
                ++j;
            }
        }
    }
    
    //подсчитаем расстояние хемминга для полученных строк
    int hem = 0;
    for(int i = 0; i < s_size; ++i){
        s[i] = A[i];
    }
    for(int i = s_size; i < s_size + p_size; ++i){
        p[i - s_size] = A[i];
    }
    for(int d = 0; d < s_size - p_size + 1; ++d){
        for(int h = 0; h < p_size; ++h){
            if(s[d + h] != p[h]){
                ++hem;
            }
        }
    }
    
    std::cout << hem << '\n';
    for(int i = 0; i < s_size + p_size; ++i){
        if(i == s_size){
            std::cout << '\n';
        }
        std::cout << A[i];
    }
    std::cout << '\n';
    
    return 0;
}
