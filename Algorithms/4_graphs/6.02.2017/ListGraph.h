#include <vector>
#include <list>
#include "IGraph.h"
#include <stack>
#pragma once
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
    std::vector<std::list<int>> adjacentList;
    std::vector<std::list<int>> PrevAdjacentList;

};
