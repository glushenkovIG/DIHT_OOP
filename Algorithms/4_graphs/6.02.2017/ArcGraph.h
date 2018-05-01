//
//  CArcGraph.hpp
//  6.02.2017
//
//  Created by Ivan on 11.02.17.
//  Copyright © 2017 Ivan. All rights reserved.
//
#pragma once
#include <vector>
#include "IGraph.h"
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
    std::vector<std::pair<int, int>> Edges;
};

