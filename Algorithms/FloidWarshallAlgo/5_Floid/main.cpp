//
//  main.cpp
//  5_Floid
//
//  Created by Ivan on 09.03.17.
//  Copyright © 2017 Ivan. All rights reserved.
//

//
//  main.cpp
//  B_Arbitrage
//
//  Created by Ivan on 06.03.17.
//  Copyright © 2017 Ivan. All rights reserved.
//

#include <iostream>
#include <vector>
#include "fstream"
class IGraph {
public:
    virtual ~IGraph() {}
    
    virtual void AddEdge(int from, int to, long long time) = 0;
    
    virtual bool HasEdge(int from, int to) = 0;
    
    virtual int VerticesCount() const = 0;
    
    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const = 0;
    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const = 0;
    
    virtual long long GetTime(int from, int to) const = 0;
    
};

class MatrixGraph: public IGraph{
public:
    MatrixGraph(int);
    MatrixGraph(const IGraph*);
    ~MatrixGraph(){}
    virtual void AddEdge(int from, int to, long long time);
    
    virtual bool HasEdge(int from, int to);
    
    virtual int VerticesCount() const;
    
    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const;
    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const;
    
    virtual long long GetTime(int from, int to) const;
    
    
private:
    std::vector<std::vector<long long> > Matrix;
};

MatrixGraph::MatrixGraph(int count){
    Matrix.resize(count);
    for(int i = 0; i < count; ++i){
        Matrix[i].resize(count, INT_MAX);
    }
}

int MatrixGraph::VerticesCount() const{
    return int(Matrix.size());
}

void MatrixGraph::AddEdge(int from, int to, long long time){
    Matrix[from][to] = time;
}

bool MatrixGraph::HasEdge(int from, int to){
    if(Matrix[from][to] != INT_MAX){
        return true;
    }
    return false;
}

void MatrixGraph::GetNextVertices(int from, std::vector<int>& vertices) const{
    for(int i = 0; i < Matrix.size(); ++i){
        if(Matrix[from][i] != INT_MAX){
            vertices.push_back(i);
        }
    }
}

void MatrixGraph::GetPrevVertices(int to, std::vector<int>& vertices) const{
    for(int i = 0; i < Matrix.size(); ++i){
        if(Matrix[i][to] != INT_MAX){
            vertices.push_back(i);
        }
    }
}

long long MatrixGraph::GetTime(int from, int to) const{
    return Matrix[from][to];
}


void Floid(IGraph& graph){
    int n = graph.VerticesCount();
    for (int k = 0; k < n; ++k){
        for (int i = 0; i < n; ++i){
            for (int j = 0; j < n; ++j){
                graph.AddEdge(i, j, std::min(graph.GetTime(i, j), graph.GetTime(i, k) + graph.GetTime(k, j)));
            }
        }
    }
}


int main(int argc, const char * argv[]) {
    std::ifstream fin("floyd.in");
    std::ofstream fout("floyd.out");
    int n = 0;
    std::cin >> n;
    MatrixGraph graph(n);
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            long long a;
            std::cin >> a;
            graph.AddEdge(i, j, a);
        }
    }
    Floid(graph);
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            std::cout << graph.GetTime(i, j) << ' ';
        }
        std::cout << '\n';
    }
    fin.close();
    fout.close();
    return 0;
}
