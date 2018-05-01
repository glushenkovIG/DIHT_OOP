//
//  main.cpp
//  Stroki
//
//  Created by Ivan on 27.09.17.
//  Copyright © 2017 Ivan. All rights reserved.
//

#include <iostream>
#include "CStrings.h"

int main(){
    std::string s;
    std::cin >> s;
    
    std::vector<int> P = string_to_preff(s);
    std::cout << "string_to_preff: ";
    print_vector(P);
    std::cout << "preff_to_string: " << preff_to_string(P) << '\n';
    
    std::vector<int> Z = string_to_zet(s);
    std::cout << "string_to_zet: ";
    print_vector(Z);
    std::cout << "zet_to_string: " << zet_to_string(Z) <<'\n';
    
    std::vector<int> P_Z = preff_to_zet(P);
    std::cout << "preff_to_zet: ";
    print_vector(P_Z);
    
    std::vector<int> Z_P = zet_to_preff(Z);
    std::cout << "zet_to_pref: ";
    print_vector(Z_P);
}
