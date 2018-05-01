#include "ListGraph.h"

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
    for (std::list<int>::iterator it = fromList.begin(); it != fromList.end(); ++it){
        vertices.push_back(*it);
    }
}

void ListGraph::GetPrevVertices(int to, std::vector<int> &vertices) const{
    std::list<int> fromList = PrevAdjacentList[to];
    for (std::list<int>::iterator it = fromList.begin(); it != fromList.end(); ++it){
        vertices.push_back(*it);
    }
}







