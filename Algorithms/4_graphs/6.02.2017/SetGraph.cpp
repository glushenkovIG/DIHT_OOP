//
//  SetGraph.cpp
//  Graph
//
//  Created by Ivan on 11.02.17.
//  Copyright © 2017 Ivan. All rights reserved.
//

#include "SetGraph.h"

SetGraph::SetGraph(const IGraph* graph){
    SetGraph newGraph (graph->VerticesCount());
    std::vector<int> A;
    for(int i = 0; i < graph->VerticesCount(); ++i){
        GetNextVertices(i, A);
        for(int j = 0; j < A.size(); ++j){
            Table[i].insert(A[j]);
        }
        A.clear();
    }
}

SetGraph::SetGraph(int count){
    Table.resize(count);
}

int SetGraph::VerticesCount() const {
    return Table.size();
}

void SetGraph::AddEdge(int from, int to){
    Table[from].insert(to);
    PrevTable[to].insert(from);
}

bool SetGraph::HasEdge(int from, int to){
    if(Table[from].find(to) != Table[from].end()){
        return true;
    }
    return false;
}

void SetGraph::GetNextVertices(int from, std::vector<int>& vertices) const{
    for(std::unordered_set<int>::const_iterator it = Table[from].begin(); it != Table[from].end(); it++){      vertices.push_back(*it);
    }
}

void SetGraph::GetPrevVertices(int to, std::vector<int>& vertices) const{
    for(std::unordered_set<int>::const_iterator it = PrevTable[to].begin(); it != PrevTable[to].end(); it++){      vertices.push_back(*it);
    }
}

