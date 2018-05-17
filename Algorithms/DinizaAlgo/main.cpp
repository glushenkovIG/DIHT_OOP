//
//  main.cpp
//  3_Diniza
//  Created by Ivan on 24.04.17.
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
        Matrix[i].resize(count);
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
        if(Matrix[from][i]){
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

int getMaxFlow(int from, int to, const IGraph& Capacity) {
    int max_flow = 0;
    int n = Capacity.VerticesCount();
    std::vector<int> level(n, -1);// список уровней для слоистой сети
    std::vector<int> lastDelVert(n, 0); // массив для хранения последней удалённой вершины при поиске блок потока
    MatrixGraph CurrFlow = MatrixGraph(n);// граф текущего потока из from в to

    //пока существует путь то значит можно пустить поток какого то ненулевого веса
    while(bfs(from, to, n, level, Capacity, CurrFlow)) {
        lastDelVert.clear();
        lastDelVert.resize(n, 0);
        
        while (int d_flow = dfs(from, 100000000, from, to, n, level, lastDelVert, Capacity, CurrFlow)){
            max_flow += d_flow;
        }
    }
    // пути больше нет значит мы нашли максимальный поток
    return max_flow;
}

int main(){
    std::ifstream fin("maxflow.in");
    std::ofstream fout("maxflow.out");
    
    int n, m;
    fin >> n >> m;
    MatrixGraph Capacity = MatrixGraph(n); // граф пропускных способностей
    
    int begin, end, weight;
    for (int i = 0; i < m; ++i){
        fin >> begin >> end >> weight;
        // нумерация у меня будет с 0 до n - 1 так как так удобнее
        Capacity.AddEdge(begin - 1, end - 1, weight);
    }
    
    int from = 0;
    int to = n - 1;
    fout << getMaxFlow(from, to, Capacity);
    
    fin.close();
    fout.close();
    return 0;
}







