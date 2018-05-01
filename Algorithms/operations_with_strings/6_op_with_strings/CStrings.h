//
//  CStrings.h
//  Stroki
//
//  Created by Ivan on 27.09.17.
//  Copyright © 2017 Ivan. All rights reserved.
//
#pragma once

#ifndef CStrings_h
#define CStrings_h

#include <vector>
#include <string>


std::vector<int> string_to_preff( const std::string& S );
std::vector<int> string_to_zet( const std::string& S );


std::vector<int> preff_to_zet( const std::vector<int>& p );
std::vector<int> zet_to_preff( const std::vector<int>& z );

std::string zet_to_string( const std::vector<int>& z );
std::string preff_to_string( const std::vector<int>& p );

void print_vector(const std::vector<int>& p);

#endif /* CStrings_h */
