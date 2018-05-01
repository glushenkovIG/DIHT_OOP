//
//  main.cpp
//  6.02.2017
//
//  Created by Ivan on 06.02.17.
//  Copyright © 2017 Ivan. All rights reserved.
//

#include <iostream>
#include <vector>
#include <list>
#include "ListGraph.h"
#include "ArcGraph.h"
#include "MatrixGraph.h"
#include <stack>


int main(int argc, char* argv[]) {
    int v = 0;
    int e = 0;
    std::cin >> v >> e;
    ListGraph graph(v);
    for (int i = 0; i < e; ++i) {
        int from = 0;
        int to = 0;
        std::cin >> from >> to;
        graph.AddEdge(from, to);
    }
    return 0;
}



