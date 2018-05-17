//
//  main.cpp
//  1_Boruvka
//
//  Created by Ivan on 08.04.17.
//  Copyright © 2017 Ivan. All rights reserved.
//


#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

struct SEdge{
    int from, to, weight;
    SEdge(int from_, int to_, int weight_);
};

SEdge::SEdge(int from_, int to_, int weight_) :
    from(from_),
    to(to_),
    weight(weight_)
{
}

class IGraph {
public:
    virtual ~IGraph() {}
    
    virtual void AddEdge(int from, int to, int time) = 0;
    
    virtual int VerticesCount() const = 0;
    
    virtual void GetNextVertices(int vertex, std::vector<SEdge>& vertices) const = 0;
    
    virtual void GetAllEdges(std::vector<SEdge>& edges) const = 0;
};

class ArcGraph: public IGraph{
public:
    ArcGraph(int _verticesCount);
    ~ArcGraph(){}
    
    void AddEdge(int from, int to, int time) override;
    
    int VerticesCount() const override;
    
    void GetNextVertices(int vertex, std::vector<SEdge>& vertices) const override;
    
    void GetAllEdges(std::vector<SEdge>& edges) const override;

private:
    const int verticesCount;
    std::vector<SEdge> vectorEdges;
};

ArcGraph::ArcGraph(int _verticesCount):
    verticesCount(_verticesCount)
{
}

void ArcGraph::GetAllEdges(std::vector<SEdge>& edges) const{
    edges = vectorEdges;
}

int ArcGraph::VerticesCount() const{
    return verticesCount;
}

void ArcGraph::AddEdge(int from, int to, int weight){
    vectorEdges.push_back(SEdge(from, to, weight));
    vectorEdges.push_back(SEdge(to, from, weight));
}

void ArcGraph::GetNextVertices(int from, std::vector<SEdge>& vertices) const {
    vertices.clear();
    for(const SEdge& edge: vectorEdges)
        if(edge.from == from)
            vertices.push_back(edge);
}

class CDsu{
public:
    CDsu(int count);
    void make_part(int vertex);
    int find_part(int vertex);
    void union_parts(int a, int b);
private:
    std::vector<int> parents;
};

CDsu::CDsu(int n) :
    parents(n)
{
    for(int i = 0; i < n; ++i){
        parents[i] = i;
    }
}
void CDsu::make_part (int vertex) {
    parents[vertex] = vertex;
}

int CDsu::find_part (int vertex) {
    if (vertex == parents[vertex])
        return vertex;
    return parents[vertex] = find_part(parents[vertex]);
}

void CDsu::union_parts (int a, int b) {
    a = find_part (a);
    b = find_part (b);
    if (a != b)
        parents[b] = a;
}

int getWeighMST(const IGraph& graph){
    std::vector<SEdge> Edges;
    graph.GetAllEdges(Edges);
    int cost = 0;
    sort (Edges.begin(), Edges.end(), [](const SEdge& a, const SEdge& b) { return a.weight < b.weight; });
    CDsu DSU = CDsu(graph.VerticesCount());
    for(const SEdge& edge: Edges){
        if (DSU.find_part(edge.from) != DSU.find_part(edge.to))
        {
            cost += edge.weight;
            DSU.union_parts(edge.from, edge.to);
        }
    }
    return cost;
}

int main(){
    std::ifstream fin("kruskal.in");
    std::ofstream fout("kruskal.out");
    int n, m;
    fin >> n >> m;
    ArcGraph graph = ArcGraph(n);
    for(int i = 0; i < m; ++i){
        int b, e, w;
        fin >> b >> e >> w;
        graph.AddEdge(b - 1, e - 1, w);
    }
    fout << getWeighMST(graph) << '\n';
    fin.close();
    fout.close();
    return 0;
}
