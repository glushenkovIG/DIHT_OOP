//
//  main.cpp
//  3_polygon_cross
//
//  Created by Ivan on 27.11.17.
//  Copyright © 2017 Ivan. All rights reserved.
//

#include <iostream>
#include <math.h>
#include <vector>

struct SMyPoint{
    SMyPoint(): x(0), y(0) {};
    SMyPoint(double x_, double y_) : x(x_), y(y_) {};
    SMyPoint(const SMyPoint& p) : x(p.x), y(p.y) {};
    
    void print(){ std::cout << x << '|' << y << '\n';};
    
    SMyPoint operator -(const SMyPoint& b) const {return SMyPoint(x - b.x, y - b.y);}
    SMyPoint operator -(){return SMyPoint(-x, -y);}
    SMyPoint operator +(const SMyPoint& b) const {return SMyPoint(x + b.x, y + b.y);}
    
    double x, y;
};

double distance(const SMyPoint& a, const SMyPoint& b){
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
};

// This function helps us to understand is point in polygon or not
// we just go all vertex and if [V-next_V, V-Zero]  has ALWAYS positive (or negative)
// it means that zero is in polygon.
double get_direction(int a, const std::vector<SMyPoint>& polygon){
    int n = static_cast<int> (polygon.size());
    
    double
    x1 = polygon[(a + 1) % n].x - polygon[a].x,
    y1 = polygon[(a + 1) % n].y - polygon[a].y,
    
    x2 = -polygon[a].x,
    y2 = -polygon[a].y;
    return x1 * y2 - y1 * x2;
}

int get_first_point(const std::vector<SMyPoint>& polygon){
    int first_point = -1;
    double min_y = 100000;
    double curr_y = -1;
    for(int i = 0; i < polygon.size(); ++i){
        curr_y = polygon[i].y;
        if(curr_y < min_y || (curr_y == min_y && polygon[i].x > polygon[min_y].x)){
            min_y = curr_y;
            first_point = i;
        }
    }
    return first_point;
}

// get angle between current point and the next one
double get_angle(int i, const std::vector<SMyPoint>& polygon){
    int n = static_cast<int> (polygon.size());
    
    double x = polygon[(i + 1) % n].x - polygon[i].x;
    double y = polygon[(i + 1) % n].y - polygon[i].y;
    // we need angle in the clockwise
    if(y < 0){
        return  - acos(x / sqrt(pow(x, 2) + pow(y, 2)));
    }
    return acos(x / sqrt(pow(x, 2) + pow(y, 2)));
}

// is point (0, 0) in our polygon
bool is_zero_zero_in_polygon(const std::vector<SMyPoint>& polygon){
    double direction = get_direction(0, polygon);
    for(int i = 1; i < polygon.size() - 1; ++i){
        if((get_direction(i, polygon) <= 0) != (direction <= 0)){
            return false;
        }
    }
    return true;
}

bool is_polygonA_cross_polygonB(const std::vector<SMyPoint>& polygonA, const std::vector<SMyPoint>& polygonB){
    std::vector<SMyPoint> polygonAB;
    int first_pointA = get_first_point(polygonA);
    int first_pointB = get_first_point(polygonB);
    SMyPoint first_point = polygonA[first_pointA] + polygonB[first_pointB];
    
    polygonAB.push_back(first_point);
    
    int n = static_cast<int> (polygonA.size());
    int m = static_cast<int> (polygonB.size());
    
    int it_A = first_pointA, it_B = first_pointB;
    bool begin_flagA = false;
    bool begin_flagB = false;
    
    while((!begin_flagA || it_A != first_pointA) && (it_B != first_pointB || !begin_flagB)){
        double angleA = get_angle(it_A, polygonA);
        double angleB = get_angle(it_B, polygonB);
        
        if(angleA > angleB){
            polygonAB.push_back(polygonA[(it_A + 1) % n] - polygonA[it_A] + polygonAB[polygonAB.size() - 1]);
            it_A = (it_A + 1) % n; // because we can start now with the first point
            begin_flagA = true;
        }
        else{
            polygonAB.push_back(polygonB[(it_B + 1) % m] - polygonB[it_B] + polygonAB[polygonAB.size() - 1]);
            it_B = (it_B + 1) % m;
            begin_flagB = true;
        }
    }
    if(it_A != first_pointA){
        while (it_A != first_pointA) {
            polygonAB.push_back(polygonA[(it_A + 1) % n] - polygonA[it_A] + polygonAB[polygonAB.size() - 1]);
            it_A = (it_A + 1) % n;
        }
    }
    else if(it_B != first_pointB){
        while (it_B != first_pointB) {
            polygonAB.push_back(polygonB[(it_B + 1) % m] - polygonB[it_B] + polygonAB[polygonAB.size() - 1]);
            it_B = (it_B + 1) % m;
        }
    }
    return is_zero_zero_in_polygon(polygonAB);
}

int main(int argc, const char * argv[]) {
    int n;
    std::cin >> n;
    double x, y;
    std::vector<SMyPoint> polygonA;
    for(int i = 0; i < n; ++i){
        std::cin >> x >> y;
        polygonA.emplace_back(x, y);
    }
    int m;
    std::cin >> m;
    std::vector<SMyPoint> polygonB;
    std::vector<SMyPoint> polygonB1;
    
    for(int i = 0; i < m; ++i){
        std::cin >> x >> y;
        polygonB.emplace_back(-x, -y);
        
    }
    std::cout << (is_polygonA_cross_polygonB(polygonA, polygonB) ? "YES" : "NO");
    return 0;
}
