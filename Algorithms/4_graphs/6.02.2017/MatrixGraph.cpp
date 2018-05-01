//
//  CMatrixGraph.cpp
//  6.02.2017
//
//  Created by Ivan on 11.02.17.
//  Copyright © 2017 Ivan. All rights reserved.
//

#include "MatrixGraph.h"
#include <vector>

MatrixGraph::MatrixGraph(const IGraph* graph){
    MatrixGraph newGraph (graph->VerticesCount());
    std::vector<int> A;
    for(int i = 0; i < graph->VerticesCount(); ++i){
        GetNextVertices(i, A);
        for(int j = 0; j < A.size(); ++j){
            Matrix[i][A[j]] = 1;
        }
        A.clear();
    }
}

MatrixGraph::MatrixGraph(int count){
    Matrix.resize(count);
}

int MatrixGraph::VerticesCount() const{
    return int(Matrix.size());
}

void MatrixGraph::AddEdge(int from, int to){
    Matrix[from][to] = 1;
}

bool MatrixGraph::HasEdge(int from, int to){
    if(Matrix[from][to] == 1){
        return true;
    }
    return false;
}

void MatrixGraph::GetNextVertices(int from, std::vector<int>& vertices) const{
    for(int i = 0; i < Matrix.size(); ++i){
        if(Matrix[from][i]){
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

