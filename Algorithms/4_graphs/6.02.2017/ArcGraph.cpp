//
//  CArcGraph.cpp
//  6.02.2017
//
//  Created by Ivan on 11.02.17.
//  Copyright © 2017 Ivan. All rights reserved.
//

#include "ArcGraph.h"

ArcGraph::ArcGraph(int count){
    Edges.resize(count);
}

ArcGraph::ArcGraph(const IGraph* graph){
    ArcGraph newGraph(graph->VerticesCount());
    std::vector<int> A;
    for(int i = 0; i < graph->VerticesCount(); ++i){
        GetNextVertices(i, A);
        for(int j = 0; j < A.size(); ++j){
            Edges.push_back(std::pair<int, int> (i, A[i]));
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
    }
}

void ArcGraph::GetPrevVertices(int to, std::vector<int>& vertices) const{
    for(int i = 0; i < Edges.size(); ++i){
        if(Edges[i].second == to){
            vertices.push_back(Edges[i].first);
        }
    }
}

