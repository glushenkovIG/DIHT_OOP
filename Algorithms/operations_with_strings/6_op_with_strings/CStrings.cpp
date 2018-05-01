//
//  CStrings.cpp
//  Stroki
//
//  Created by Ivan on 27.09.17.
//  Copyright © 2017 Ivan. All rights reserved.
//


#include <iostream>
#include "CStrings.h"

std::vector<int> string_to_preff(const std::string& str){
    std::vector<int> p(str.size());
    int i = 1;
    while( i < str.size() ){
        int curr = i;
        while(curr != 0){
            if( str[i] == str[p[curr - 1]] ){
                p[i] = p[curr - 1] + 1;
                break;
            }
            curr = p[curr - 1];
        }
        if( curr == 0 ){
            p[i] = 0;
        }
        ++i;
    }
    return p;
}

std::vector<int> string_to_zet(const std::string& S){
    int n = static_cast<int>(S.size());
    std::vector<int> z(n);
    
    for (int i = 1, l = 0, r = 0; i < n; ++i) {
        if(r >= i){
            z[i] = std::min(z[i - l], r - i + 1);
        }
        while(i + z[i] < n && S[i + z[i]] == S[z[i]]){
            ++z[i];
        }
        if (i + z[i] - 1 > r){
            r = i + z[i] - 1;
            l = i;
        }
    }
    return z;
}

std::vector<int> preff_to_zet(const std::vector<int>& p){
    std::string s = preff_to_string(p);// по транзитивности
    return string_to_zet(s);
}

std::vector<int> zet_to_preff(const std::vector<int>& z){
    int n = static_cast<int>(z.size());
    std::vector<int> Preff(n);
    
    for(int i = 1; i < n; i++){
        if(z[i]){
            int j = z[i] - 1;
            while( j >= 0 && !(Preff[i + j]) ){
                Preff[i + j] = j + 1;
                --j;
            }
        }
    }
    return Preff;
}

std::string zet_to_string(const std::vector<int>& z){
    std::vector<int> p = zet_to_preff(z);
    return preff_to_string(p);
}

std::string preff_to_string(const std::vector<int>& p){
    std::string s = "";
    int n = static_cast<int>(p.size());
    std::vector<int> A(n);
    for(int i = 0; i < n; ++i){
        if(p[i] != 0){
            s += s[p[i] - 1];
        }
        else{
            int k = i - 1;
            while(k >= 0){
                A[s[p[k]] - 'a'] = 1;
                k = p[k] - 1;
            }
            k = 0;
            while (A[k] != 0) {
                ++k;
            }
            s += 'a' + k;
            A.assign(A.size(), 0);
        }
    }
    return s;
}

void print_vector(const std::vector<int>& A){
    for(int i = 0; i < A.size(); ++i){
        std::cout << A[i] << '|';
    }
    std::cout << '\n';
}
