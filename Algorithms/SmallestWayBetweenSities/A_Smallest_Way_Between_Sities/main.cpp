//
//  main.cpp
//  A_Smallest_Way_Between_Sities
//
//  Created by Ivan on 06.03.17.
//  Copyright © 2017 Ivan. All rights reserved.
//

#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include "unordered_set"
#include <map>
#include <set>
#include "math.h"
#include "limits.h"

class IGraph {
public:
    virtual ~IGraph() {}
    
    // Добавление ребра от from к to.
    
    virtual void AddEdge(int from, int to, int time) = 0;
    
    virtual bool HasEdge(int from, int to) = 0;
    
    virtual int VerticesCount() const = 0;
    
    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const = 0;
    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const = 0;
    virtual int GetTime(int from, int to) const = 0;

};

class MatrixGraph: public IGraph{
public:
    MatrixGraph(int);
    MatrixGraph(const IGraph*);
    ~MatrixGraph(){}
    virtual void AddEdge(int from, int to, int time);
    
    virtual bool HasEdge(int from, int to);
    
    virtual int VerticesCount() const;
    
    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const;
    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const;
    virtual int GetTime(int from, int to) const;

    
private:
    std::vector<std::vector<int> > Matrix;
};

MatrixGraph::MatrixGraph(int count){
    Matrix.resize(count);
    for(int i = 0; i < count; ++i){
        Matrix[i].resize(count, 0);
    }
}

int MatrixGraph::VerticesCount() const{
    return int(Matrix.size());
}

void MatrixGraph::AddEdge(int from, int to, int time){
    Matrix[from][to] = time;
    Matrix[to][from] = time;
}

bool MatrixGraph::HasEdge(int from, int to){
    if(Matrix[from][to] != 0){
        return true;
    }
    return false;
}

void MatrixGraph::GetNextVertices(int from, std::vector<int>& vertices) const{
    for(int i = 0; i < Matrix.size(); ++i){
        if(Matrix[from][i] != 0){
            vertices.push_back(i);
        }
    }
}

void MatrixGraph::GetPrevVertices(int to, std::vector<int>& vertices) const{
    for(int i = 0; i < Matrix.size(); ++i){
        if(Matrix[i][to] != 0){
            vertices.push_back(i);
        }
    }
}

int MatrixGraph::GetTime(int from, int to) const{
    return Matrix[from][to];
}

bool isLessS(const int &a, const int &b){
    return a < b;
}

template<class T>

void SiftDown(T* arr, int i, int n, bool (*isLess)(const T&, const T&))
{
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    int largest = i;
    if( left < n && isLess(arr[i], arr[left])){
        largest = left;
    }
    
    if( right < n && isLess(arr[largest], arr[right]) ){
        largest = right;
    }
    
    if( largest != i ) {
        std::swap( arr[i], arr[largest] );
        SiftDown( arr, largest , n, isLess);
    }
}

template<class T>
void BuildHeap(T* arr, int n, bool (*isLess)(const T&, const T&)){
    for(int i = n / 2 - 1; i >= 0; i--){
        SiftDown(arr, i, n, isLess);
    }
}

template<class T>

void HeapSort( T* arr, int l, int r , bool (*isLess)(const T&, const T&)) {
    int heapSize = r - l;
    BuildHeap( arr, heapSize, isLess);
    while( heapSize > 1 ) {
        std::swap( arr[0], arr[heapSize - 1] );
        --heapSize;
        SiftDown(arr, 0, heapSize, isLess);
    }
}



int getMinWayBetweenSities(IGraph* graph, int from, int to){
    int inf = INT_MAX;
    std::vector<int> Ways(graph->VerticesCount(), inf);
    std::set<std::pair<int, int> > Way;
    std::vector<bool> isVisited(graph->VerticesCount(), false);
    for(int i = 0; i < graph->VerticesCount(); ++i){
        if (i != from){
            Way.insert(std::make_pair(inf, i));
        }
        else{
            Way.insert(std::make_pair(0, from));
        }
    }
    
    Ways[from] = 0;
    int min_node = 0;
    while(min_node != -1){
        int minWay = INT_MAX;
        int min_node = -1;
        min_node = (*Way.begin()).second;
        if(Way.empty()){
            return Ways[to];
        }
        isVisited[min_node] = true;
        Way.erase(std::make_pair(Ways[min_node], min_node));

        std::vector<int> A;
        graph->GetNextVertices(min_node, A);
        for(int i = 0; i < A.size(); ++i){
            if(Ways[A[i]] > Ways[min_node] + graph->GetTime(min_node, A[i])){
                Way.erase(std::make_pair(Ways[A[i]], A[i]));
                Ways[A[i]] = Ways[min_node] + graph->GetTime(min_node, A[i]);
                Way.insert(std::make_pair(Ways[A[i]], A[i]));
            }
        }
    }
    return 0;
}

int main() {
    int n = 0;
    int v = 0;
    std::cin >> n >> v;
    MatrixGraph* graph;
    for(int i = 0; i < v; ++i){
        int from, to, time;
        std::cin >> from >> to >> time;
        graph->AddEdge(from, to, time);
    }
    int from, to;
    std::cin >> from >> to;
    std::cout << getMinWayBetweenSities(graph, from, to);
    return 0;
}























