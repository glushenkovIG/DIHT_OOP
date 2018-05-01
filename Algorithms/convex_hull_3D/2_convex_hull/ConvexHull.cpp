//
//  ConvexHull.cpp
//  3d_cover
//
//  Created by Ivan on 02.12.17.
//  Copyright © 2017 Ivan. All rights reserved.
//

#include "ConvexHull.h"

#include <set>

long long scalar_product(const SMySegment& segm1, const SMySegment& segm2){
    const long long
    x1 = segm1.end.x - segm1.begin.x,
    y1 = segm1.end.y - segm1.begin.y,
    z1 = segm1.end.z - segm1.begin.z,
    
    x2 = segm2.end.x - segm2.begin.x,
    y2 = segm2.end.y - segm2.begin.y,
    z2 = segm2.end.z - segm2.begin.z;
    
    return x1 * x2 + y1 * y2 + z1 * z2;
}

SMySegment vector_product(const SMySegment& segm1, const SMySegment& segm2){
    int
    x1 = segm1.end.x - segm1.begin.x,
    y1 = segm1.end.y - segm1.begin.y,
    z1 = segm1.end.z - segm1.begin.z,
    
    x2 = segm2.end.x - segm2.begin.x,
    y2 = segm2.end.y - segm2.begin.y,
    z2 = segm2.end.z - segm2.begin.z;
    
    SMySegment segm12;
    segm12.begin = SMyPoint();
    
    segm12.end.x = y1 * z2 - z1 * y2;
    segm12.end.y = z1 * x2 - x1 * z2;
    segm12.end.z = x1 * y2 - y1 * x2;
    
    segm12.len = distance(segm12.begin, segm12.end);
    
    return segm12;
}

SMySegment SMySide::get_inner_norm() const{
    SMySegment norm = vector_product(SMySegment(points[a], points[b]), SMySegment(points[a], points[c]));
    int not_abc = 0;
    while(not_abc == a || not_abc == b || not_abc == c){
        ++not_abc;
    }
    return (scalar_product(norm, SMySegment(SMyPoint(), points[not_abc] - points[a])) > 0 ? norm : -norm);
}

SMySide SMySide::sort() const {
    long long
    A = std::min(a, std::min(b, c)),
    C = std::max(a, std::max(b, c)),
    B = a + b + c - A - C;
    
    SMySegment norm = (*this).get_inner_norm();
    SMySegment AB(points[A], points[B]);
    SMySegment BC(points[B], points[C]);
    
    return (scalar_product(vector_product(norm, BC), AB) > 0 ? SMySide(A, B, C) : SMySide(A, C, B));
}

void ConvexHull::print(){
    std::cout << sides.size() << '\n';
    for(auto elem: sides){
        std::cout << "3 " << elem.a << " " << elem.b << " " << elem.c << '\n';
    }
}

// searching the point A, which has the biggest angle with the previous side
// O(|n|)
void ConvexHull::get_base_side(){
    int base_point1 = -1;// point with the smallest z coordinate
    int min_z = 1001;
    for(int i = 0; i < points.size(); ++i){
        if(points[i].z < min_z){
            min_z = points[i].z;
            base_point1 = i;
        }
    }
    // point with the biggest angle from the base_point1, this point will have the smaller proection to Z coordinate
    int base_point2 = -1;
    double min_z_proection = 0;
    for(int i = 0; i < points.size(); ++i){
        double z_proection = get_angle_between_segm(SMySegment(points[base_point1], points[i]), SMySegment(SMyPoint(), SMyPoint(0, 0, 1)));
        if(i != base_point1 && z_proection > min_z_proection){
            min_z_proection = z_proection;
            base_point2 = i;
        }
    }
    // all other points should be on the one half-space
    int base_point3 = -1;
    for(int i = 0; i < points.size(); ++i){
        if(i != base_point1 && i != base_point2 && are_all_points_oneside(i, base_point1, base_point2)){
            base_point3 = i;
        }
    }
    sides.emplace_back(base_point1, base_point2, base_point3);
}

ConvexHull::ConvexHull(){
    get_base_side();
    std::set<SMySide> border;
    border.emplace(sides[0].a, sides[0].b, sides[0].c);
    border.emplace(sides[0].a, sides[0].c, sides[0].b);
    border.emplace(sides[0].c, sides[0].b, sides[0].a);
    while (!border.empty()) {
        std::set<SMySide>::const_iterator it = border.begin();
        SMySide cSide = *it;
        border.erase(it);
        int curr_point = get_next_point(cSide);
        
        SMySide ABC(curr_point, cSide.a, cSide.b);
        border.count(ABC) == 0 ? void(border.insert(ABC)) : void(border.erase(ABC));
        
        SMySide ACB(curr_point, cSide.b, cSide.a);
        border.count(ACB) == 0 ? void(border.insert(ACB)) : void(border.erase(ACB));
        
        sides.push_back(ABC.sort());
    }
    sort_sides();
}


int get_next_point(const SMySide& side){
    double min_angle = 10;
    int next_point = -1;
    SMySegment inner_norm = side.get_inner_norm();
    for(int i = 0; i < points.size(); ++i){
        if(i != side.a && i != side.b && i != side.c){
            SMySegment AI(points[side.a], points[i]);
            SMySegment norm1 = vector_product(SMySegment(points[side.a], points[side.c]), SMySegment(points[side.a], points[side.b]));
            SMySegment norm2 = vector_product(SMySegment(points[side.a], points[side.b]), AI);
            if(scalar_product(norm1, inner_norm) < 0){
                norm1 = -norm1;
                norm2 = -norm2;
            }
            double angle = get_angle_between_segm(norm1, norm2);
            if(angle < min_angle){
                min_angle = angle;
                next_point = i;
            }
        }
    }
    return next_point;
}

// sort sides in lexicographic order and in the clockwise order if we look inside
void ConvexHull::sort_sides(){
    for(int i = 0; i < sides.size(); ++i){
        SMySide& side_ = sides[i];
        const long long
        A = std::min(side_.a, std::min(side_.b, side_.c)),
        C = std::max(side_.a, std::max(side_.b, side_.c)),
        B = side_.a + side_.b + side_.c - A - C;
        
        SMySegment norm = side_.get_inner_norm();
        SMySegment AB(points[A], points[B]);
        SMySegment BC(points[B], points[C]);
        
        if(scalar_product(vector_product(norm, BC), AB) > 0){
            side_.a = A;
            side_.b = B;
            side_.c = C;
        }
        else{
            side_.a = A;
            side_.b = C;
            side_.c = B;
        }
    }
    std::sort(sides.begin(), sides.end(), [](const SMySide& s1, const SMySide& s2){
        if(s1.a < s2.a) return true;
        else if (s1.a == s2.a && s1.b < s2.b) return true;
        else if (s1.a == s2.a && s1.b == s2.b) return s1.c < s2.c;
        return false;
    });
}
