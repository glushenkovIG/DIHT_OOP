//
//  SetGraph.hpp
//  Graph
//
//  Created by Ivan on 11.02.17.
//  Copyright © 2017 Ivan. All rights reserved.
//
#pragma once
#include <vector>
#include <unordered_set>
#include "IGraph.h"
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
    std::vector<std::unordered_set<int>> Table;
    std::vector<std::unordered_set<int>> PrevTable;

};

