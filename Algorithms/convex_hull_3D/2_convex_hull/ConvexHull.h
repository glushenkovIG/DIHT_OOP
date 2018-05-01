//
//  ConvexHull.h
//  3d_cover
//
//  Created by Ivan on 02.12.17.
//  Copyright © 2017 Ivan. All rights reserved.
//
// Creating 3d convex hull with the gift wrapping algorithm, O(|n^2|).
#ifndef ConvexHull_h
#define ConvexHull_h

#include <iostream>
#include <math.h>
#include <vector>

struct SMyPoint{
    SMyPoint(): x(0), y(0), z(0) {};
    SMyPoint(int x_, int y_, int z_) : x(x_), y(y_), z(z_) {}
    SMyPoint(const SMyPoint& p) : x(p.x), y(p.y), z(p.z) {};
    void print(){ std::cout << x << '|' << y << '|' << z << '\n';}
    
    SMyPoint operator +(const SMyPoint& b) const {return SMyPoint(x + b.x, y + b.y, z + b.z);}
    SMyPoint operator -(const SMyPoint& b) const {return SMyPoint(x - b.x, y - b.y, z - b.z);}
    SMyPoint operator -(){return SMyPoint(-x, -y, -z);}
    
    bool operator ==(const SMyPoint& p){return x == p.x && y == p.y && z == p.z;}
    
    int x, y, z;
};

extern std::vector<SMyPoint> points;

static inline double distance(const SMyPoint& a, const SMyPoint& b){
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
};

struct SMySegment{
    SMySegment() : begin(SMyPoint()), end(SMyPoint()), len(0) {};
    SMySegment(const SMyPoint& point) : begin(SMyPoint()), end(point), len(distance(SMyPoint(), point)){};
    SMySegment(const SMyPoint& a, const SMyPoint& b) : begin(a), end(b), len(distance(a, b)) {};
    SMySegment(const SMySegment& segm) : begin(segm.begin), end(segm.end), len(segm.len) {};
    
    SMyPoint begin, end;
    
    SMySegment operator-(){return SMySegment(-begin, -end);}
    SMySegment operator+(const SMyPoint& p) const {return SMySegment(begin + p, end + p);}
    
    void print(){begin.print(); end.print();};
    
    double len;
};

long long scalar_product(const SMySegment& segm1, const SMySegment& segm2);

SMySegment vector_product(const SMySegment& segm1, const SMySegment& segm2);

static inline double get_angle_between_segm(const SMySegment& norm1, const SMySegment& norm2){
    double cos_angle = scalar_product(norm1, norm2) / (norm1.len * norm2.len);
    return acos(cos_angle);
}

struct SMySide{
    SMySide(long long a_, long long b_, long long c_) : a(a_), b(b_), c(c_) {};
    bool operator <(const SMySide& side) const {
        long long
        a1 = std::min(a, b),
        a2 = std::min(side.a, side.b),
        
        b1 = a + b - a1,
        b2 = side.a + side.b - a2;
        
        if(a1 < a2) return true;
        else if(a1 == a2 && b1 < b2) return true;
        return false;
    }
    bool operator ==(const SMySide& side) const {return a == side.a && b == side.b && c == side.c;}
    void print(){std::cout << a << ' ' << b << ' ' << c << '\n';}
    
    SMySide sort() const;
    SMySegment get_inner_norm() const;
    long long a, b, c;
};

static inline bool are_points_oneside(int i, int curr, int a, int b, int c, const std::vector<SMyPoint>& points){
    SMySegment seg = vector_product(SMySegment(points[a], points[b]), SMySegment(points[a], points[c]));
    seg = seg + points[a];
    long long s1 = scalar_product(seg, SMySegment(points[a], points[i]));
    long long s2 = scalar_product(seg, SMySegment(points[a], points[curr]));
    return s1 * s2 > 0;
}

static inline bool are_all_points_oneside(int a, int b, int c){
    int curr = 0;
    while(curr == a || curr == b || curr == c){
        ++curr;
    }
    for(int i = 0; i < points.size(); ++i){
        if(i != a && i != b && i != c && i != curr && !are_points_oneside(i, curr, a, b, c, points)){
            return false;
        }
    }
    return true;
}

class ConvexHull{
public:
    ConvexHull();
    void print();
private:
    void get_base_side();
    void sort_sides();
    std::vector<SMySide> sides;
    
};

int get_next_point(const SMySide& side);

#endif /* ConvexHull_h */
