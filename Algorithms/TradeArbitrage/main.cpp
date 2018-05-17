//
//  main.cpp
//  B_Arbitrage
//
//  Created by Ivan on 06.03.17.
//  Copyright © 2017 Ivan. All rights reserved.
//

#include <iostream>
#include <vector>

    struct edge{
public:
    edge(int i, int j, double cost);
    
    int from, to;
    double time;
};

edge::edge(int _from, int _to, double _time) :
    from(_from),
    to(_to),
    time(_time)
    {
    }

class IGraph {
public:
    virtual ~IGraph() {}
    
    virtual void AddEdge(int from, int to, double time) = 0;
    
    virtual bool HasEdge(int from, int to) = 0;
    
    virtual int VerticesCount() const = 0;
    
    virtual void GetNextVertices(int vertex, std::vector<std::pair<int, double> >& vertices) const = 0;
    virtual void GetPrevVertices(int vertex, std::vector<std::pair<int, double> >& vertices) const = 0;
    virtual void GetAllEdges(std::vector<edge>& edges) const = 0;

};




class ArcGraph: public IGraph{
public:
    ArcGraph(int count);
    ArcGraph(const IGraph*);
    ~ArcGraph(){}
    
    virtual void AddEdge(int from, int to, double time);
    
    virtual bool HasEdge(int from, int to);
    
    virtual int VerticesCount() const;
    
    virtual void GetNextVertices(int vertex, std::vector<std::pair<int, double> >& vertices) const;
    virtual void GetPrevVertices(int vertex, std::vector<std::pair<int, double> >& vertices) const;
    virtual void GetAllEdges(std::vector<edge>& edges) const;
    
    

private:
    std::vector<edge> Edges;
};
ArcGraph::ArcGraph(int count){
    Edges.resize(count);
}

ArcGraph::ArcGraph(const IGraph* graph){
    std::vector<std::pair<int, double> > A;
    for(int i = 0; i < graph->VerticesCount(); ++i){
        GetNextVertices(i, A);
        for(int j = 0; j < A.size(); ++j){
            Edges.push_back(edge(i, A[i].first, A[i].second));
        }
        A.clear();
    }
}

void ArcGraph::GetAllEdges(std::vector<edge>& edges) const{
    edges = Edges;
}


int ArcGraph::VerticesCount() const{
    return int(Edges.size());
}

void ArcGraph::AddEdge(int from, int to, double time){
    Edges.push_back(edge(from, to, time));
}

bool ArcGraph::HasEdge(int from, int to){
    for(int i = 0; i < Edges.size(); ++i){
        if(Edges[i].from == from && Edges[i].to == to){
            return true;
        }
    }
    return false;
}

void ArcGraph::GetNextVertices(int from, std::vector<std::pair<int, double> >& vertices) const{
    for(int i = 0; i < Edges.size(); ++i){
        if(Edges[i].from == from){
            vertices.push_back(std::make_pair(Edges[i].to, Edges[i].time));
        }
    }
}

void ArcGraph::GetPrevVertices(int to, std::vector<std::pair<int, double> >& vertices) const{
    for(int i = 0; i < Edges.size(); ++i){
        if(Edges[i].to == to){
            vertices.push_back(std::make_pair(Edges[i].to, Edges[i].time));
        }
    }
}

bool isArbitrage(IGraph& graph, int n) {
    int INF = INT_MAX;
    unsigned long m = graph.VerticesCount();
    std::vector<double> d (n, INF);
    std::vector<edge> Edges;
    graph.GetAllEdges(Edges);
    d[0] = 1;
    for (int i = 0; i < n; ++i) {
        bool is = false;
        for (int j = 0; j < m; ++j){
            if (d[Edges[j].to] < d[Edges[j].from] * Edges[j].time) {
                d[Edges[j].to] = d[Edges[j].from] * Edges[j].time;
                is = true;
            }
        }
        if(!is){
            return false;
        }
    }
    return true;
}

int main(int argc, const char * argv[]) {
    int n = 0;
    std::cin >> n;
    ArcGraph graph(n);
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            if (i != j){
                double a;
                std::cin >> a;
                if (a != -1.0){
                    graph.AddEdge(i, j, a);
                }
            }
        }
    }
    std::cout << '\n' << (isArbitrage(graph, n)? "YES":"NO") << '\n';
    return 0;
}
