#include <vector>
#include "IGraph.h"
#pragma once
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
    std::vector<std::vector<int>> Matrix;
};
