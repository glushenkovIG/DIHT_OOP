//
//  main.cpp
//  Count_of_min_ways
//
//  Created by Ivan on 28.02.17.
//  Copyright © 2017 Ivan. All rights reserved.
//
//
//  main.cpp
//
//  main.cpp
//  A
//
//  Created by Ivan on 20.02.17.
//  Copyright © 2017 Ivan. All rights reserved.
//

#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include "unordered_set"


class IGraph {
public:
    virtual ~IGraph() {}
    
    // Добавление ребра от from к to.
    
    virtual void AddEdge(int from, int to) = 0;
    
    virtual bool HasEdge(int from, int to) = 0;
    
    virtual int VerticesCount() const = 0;
    
    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const = 0;
    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const = 0;
};

class MatrixGraph: public IGraph{
public:
    MatrixGraph(int);
    MatrixGraph(const IGraph*);
    ~MatrixGraph(){}
    virtual void AddEdge(int from, int to);
    
    virtual bool HasEdge(int from, int to);
    
    virtual int VerticesCount() const;
    
    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const;
    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const;
    
private:
    std::vector<std::vector<int> > Matrix;
};


MatrixGraph::MatrixGraph(const IGraph* graph){
    std::vector<int> A;
    for(int i = 0; i < graph->VerticesCount(); ++i){
        graph->GetNextVertices(i, A);
        for(int j = 0; j < A.size(); ++j){
            Matrix[i][A[j]] = 1;
            Matrix[A[j]][i] = 1;

        }
        A.clear();
    }
}

MatrixGraph::MatrixGraph(int count){
    Matrix.resize(count);
    for(int i = 0; i < count; ++i){
        Matrix[i].resize(count, 0);
    }
}

int MatrixGraph::VerticesCount() const{
    return int(Matrix.size());
}

void MatrixGraph::AddEdge(int from, int to){
    Matrix[from][to] = 1;
    Matrix[to][from] = 1;
}

bool MatrixGraph::HasEdge(int from, int to){
    if(Matrix[from][to] == 1){
        return true;
    }
    return false;
}

void MatrixGraph::GetNextVertices(int from, std::vector<int>& vertices) const{
    for(int i = 0; i < Matrix.size(); ++i){
        if(Matrix[from][i] == 1){
            vertices.push_back(i);
        }
    }
}

void MatrixGraph::GetPrevVertices(int to, std::vector<int>& vertices) const{
    for(int i = 0; i < Matrix.size(); ++i){
        if(Matrix[i][to]){
            vertices.push_back(i);
        }
    }
}

class ArcGraph: public IGraph{
public:
    ArcGraph(int count);
    ArcGraph(const IGraph*);
    ~ArcGraph(){}
    virtual void AddEdge(int from, int to);
    
    virtual bool HasEdge(int from, int to);
    
    virtual int VerticesCount() const;
    
    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const;
    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const;
private:
    std::vector<std::pair<int, int> > Edges;
};

ArcGraph::ArcGraph(int count){
    Edges.resize(count);
    for (int i = 0; i < count; ++i) {
        Edges[i].first = 0;
        Edges[i].second = 0;
    }
}

ArcGraph::ArcGraph(const IGraph* graph){
    std::vector<int> A;
    for(int i = 0; i < graph->VerticesCount(); ++i){
        graph->GetNextVertices(i, A);
        for(int j = 0; j < A.size(); ++j){
            Edges.push_back(std::pair<int, int> (i, A[j]));
            Edges.push_back(std::pair<int, int> (A[j], i));

        }
        A.clear();
    }
}


int ArcGraph::VerticesCount() const{
    return int(Edges.size());
}

void ArcGraph::AddEdge(int from, int to){
    std::pair<int, int> edge(from, to);
    Edges.push_back(edge);
    //Edges.push_back(std::pair<int, int> (to, from));

}

bool ArcGraph::HasEdge(int from, int to){
    for(int i = 0; i < Edges.size(); ++i){
        if(Edges[i].first == from && Edges[i].second == to){
            return true;
        }
    }
    return false;
}

void ArcGraph::GetNextVertices(int from, std::vector<int>& vertices) const{
    for(int i = 0; i < Edges.size(); ++i){
        if(Edges[i].first == from){
            vertices.push_back(Edges[i].second);
        }
        else if (Edges[i].second == from){
            vertices.push_back(Edges[i].first);
        }
    }
}

void ArcGraph::GetPrevVertices(int to, std::vector<int>& vertices) const{
    for(int i = 0; i < Edges.size(); ++i){
        if(Edges[i].second == to){
            vertices.push_back(Edges[i].first);
        }
    }
}

class SetGraph: public IGraph{
public:
    SetGraph(int count);
    SetGraph(const IGraph*);
    ~SetGraph(){}
    virtual void AddEdge(int from, int to);
    
    virtual bool HasEdge(int from, int to);
    
    virtual int VerticesCount() const;
    
    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const;
    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const;
private:
    std::vector<std::unordered_set<int> > Table;
    std::vector<std::unordered_set<int> > PrevTable;
    
};

SetGraph::SetGraph(int count){
    Table.resize(count);
    PrevTable.resize(count);
}

SetGraph::SetGraph(const IGraph* graph){
    std::vector<int> A;
    for(int i = 0; i < graph->VerticesCount(); ++i){
        graph->GetNextVertices(i, A);
        for(int j = 0; j < A.size(); ++j){
            Table[i].insert(A[j]);
            PrevTable[A[i]].insert(i);
        }
        A.clear();
    }
}

int SetGraph::VerticesCount() const {
    return Table.size();
}

void SetGraph::AddEdge(int from, int to){
    Table[from].insert(to);
    Table[to].insert(from);
    PrevTable[to].insert(from);
}

bool SetGraph::HasEdge(int from, int to){
    if(Table[from].find(to) != Table[from].end()){
        return true;
    }
    return false;
}

void SetGraph::GetNextVertices(int from, std::vector<int>& vertices) const{
    for(std::unordered_set<int>::const_iterator it = Table[from].cbegin(); it != Table[from].cend(); ++it){
        vertices.push_back(*it);
    }
}

void SetGraph::GetPrevVertices(int to, std::vector<int>& vertices) const{
    for(std::unordered_set<int>::const_iterator it = PrevTable[to].cbegin(); it != PrevTable[to].cend(); ++it){
        vertices.push_back(*it);
    }
}

class ListGraph: public IGraph{
public:
    ListGraph(int);
    ListGraph(const IGraph*);
     
    ~ListGraph(){}
    virtual void AddEdge(int from, int to);
 
    virtual bool HasEdge(int from, int to);
    
    virtual int VerticesCount() const;
 
    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const;
    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const;
private:
    std::vector<std::list<int> > adjacentList;
    std::vector<std::list<int> > PrevAdjacentList;
 
};
 
ListGraph::ListGraph(const IGraph* graph){
    std::vector<int> A;
    adjacentList.resize(graph->VerticesCount());
    for(int i = 0; i < graph->VerticesCount(); ++i){
        graph->GetNextVertices(i, A);
        for(int j = 0; j < A.size(); ++j){
            adjacentList[i].push_back(A[j]);
        }
        A.clear();
    }
}
 
ListGraph::ListGraph(int count){adjacentList.resize(count);}

int ListGraph::VerticesCount() const{
    return int(adjacentList.size());
}
 
void ListGraph::AddEdge(int from, int to){
    adjacentList[from].push_back(to);
    adjacentList[to].push_back(from);
    //PrevAdjacentList[to].push_back(from);
}
 
bool ListGraph::HasEdge(int from, int to){
    std::list<int>& fromList = adjacentList[from];
    for (std::list<int>::iterator it = fromList.begin(); it != fromList.end(); ++it){
        if (*it == to) {
            return true;
        }
    }
    return false;
}
 
void ListGraph::GetNextVertices(int from, std::vector<int> &vertices) const{
    std::list<int> fromList = adjacentList[from];
    for (std::list<int>::const_iterator it = fromList.cbegin(); it != fromList.cend(); ++it){
        vertices.push_back(*it);
    }
}
 
void ListGraph::GetPrevVertices(int to, std::vector<int> &vertices) const{
    std::list<int> fromList = PrevAdjacentList[to];
    for (std::list<int>::const_iterator it = fromList.cbegin(); it != fromList.cend(); ++it){
        vertices.push_back(*it);
    }
}

int countMinWays(int from, int to, IGraph& graph){
    std::queue<int> queue;
    queue.push(from);
    std::vector<int> Count(graph.VerticesCount(), 0);
    std::vector<int> Waves(graph.VerticesCount(), 0);
    Count[from] = 1;
    Waves[from] = 1;
    while (!queue.empty()){
        int s = queue.front();
        queue.pop();
        std::vector<int> A;
        graph.GetNextVertices(s, A);
        for(int i = 0; i < A.size(); ++i){
            if(Waves[A[i]] == 0){
                queue.push(A[i]);
                Waves[A[i]] = Waves[s] + 1;
                Count[A[i]] = Count[s];
            }
            else if(Waves[A[i]] == Waves[s] + 1){
                Count[A[i]] = Count[A[i]] + Count[s];
            }
        }
    }
    return Count[to];
}

int main() {
    int v = 0;
    int e = 0;
    std::cin >> v >> e;
    ListGraph graph(v);
    int from = 0;
    int to = 0;
    for (int i = 0; i < e; ++i) {
        std::cin >> from >> to;
        graph.AddEdge(from, to);
        from = 0;
        to = 0;
    }
    std::cin >> from >> to;
    std::cout << countMinWays(from, to, graph) << '\n';
    return 0;
}
