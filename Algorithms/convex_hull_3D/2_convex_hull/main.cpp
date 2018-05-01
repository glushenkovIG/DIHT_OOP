#include <iostream>
#include <algorithm>
#include <vector>
#include <math.h>
#include <set>
#include <cassert>

#include "ConvexHull.h"

std::vector<SMyPoint> points;

int main(int argc, const char * argv[]) {
    //int n;
    //std::cin >> n;
    //for(int i = 0; i < n; ++i){
    int point_count;
    std::cin >> point_count;
    for(int j = 0; j < point_count; ++j){
        int x, y, z;
        std::cin >> x >> y >> z;
        points.emplace_back(x, y, z);
    }
    ConvexHull Hull;
    Hull.print();
    //}
    return 0;
}
