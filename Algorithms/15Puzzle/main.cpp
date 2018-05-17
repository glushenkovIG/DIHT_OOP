//
//  main.cpp
//  15Puzzle
//
//  Created by Ivan on 25.03.17.
//  Copyright © 2017 Ivan. All rights reserved.
//
//
#include "iostream"
#include <assert.h>
#include <sstream>
#include <string>
#include "queue"
#include "math.h"
//#include "stdafx.h"
#include <fstream>
class CPosition {
public:
    explicit CPosition( const std::string& source );
    
    CPosition Up() const;
    CPosition Down() const;
    CPosition Right() const;
    CPosition Left() const;
    bool isRight(CPosition self);
    int getNullPlace();
    void setAt( int place, unsigned char value );
    unsigned long long data; // 16 ячеек по 4 бита каждая.
    
private:
    int nullPlace;
    
    unsigned char getAt( int place ) const;
};

int CPosition::getNullPlace(){
    return nullPlace;
}
const unsigned long long Masks[] = {
    0x000000000000000F,
    0x00000000000000F0,
    0x0000000000000F00,
    0x000000000000F000,
    0x00000000000F0000,
    0x0000000000F00000,
    0x000000000F000000,
    0x00000000F0000000,
    0x0000000F00000000,
    0x000000F000000000,
    0x00000F0000000000,
    0x0000F00000000000,
    0x000F000000000000,
    0x00F0000000000000,
    0x0F00000000000000,
    0xF000000000000000,
};

const unsigned long long AntiMasks[] = {
    0xFFFFFFFFFFFFFFF0,
    0xFFFFFFFFFFFFFF0F,
    0xFFFFFFFFFFFFF0FF,
    0xFFFFFFFFFFFF0FFF,
    0xFFFFFFFFFFF0FFFF,
    0xFFFFFFFFFF0FFFFF,
    0xFFFFFFFFF0FFFFFF,
    0xFFFFFFFF0FFFFFFF,
    0xFFFFFFF0FFFFFFFF,
    0xFFFFFF0FFFFFFFFF,
    0xFFFFF0FFFFFFFFFF,
    0xFFFF0FFFFFFFFFFF,
    0xFFF0FFFFFFFFFFFF,
    0xFF0FFFFFFFFFFFFF,
    0xF0FFFFFFFFFFFFFF,
    0x0FFFFFFFFFFFFFFF
};

CPosition::CPosition( const std::string& source ) :
data( 0 ),
nullPlace( 0 )
{
    std::istringstream stringStream( source );
    for( char i = 0; i < 16; ++i ) {
        unsigned short value = 0;
        stringStream >> value;
        setAt( i, static_cast<unsigned char>( value ) );
        if( value == 0 ) {
            nullPlace = i;
        }
    }
}

// Установка значения в некоторую позицию.
void CPosition::setAt( int place, unsigned char value )
{
    //unsigned long long val = static_cast<unsigned long long>( value );
    //unsigned long long val1 = static_cast<unsigned long long>( value ) << ( place << 2 );
    //unsigned long long val2 = data & AntiMasks[place];
    data = ( data & AntiMasks[place] ) | ( static_cast<unsigned long long>( value ) << ( place << 2 ) );
}

// Получение того, что лежит в некоторой позиции.
unsigned char CPosition::getAt( int place ) const
{
    return static_cast<unsigned char>( ( data & Masks[place] ) >> ( place << 2 ) );
}

CPosition CPosition::Up() const
{
    assert( nullPlace >= 4 );
    
    CPosition result( *this );
    
    // Ставим пустышку выше.
    result.setAt( nullPlace - 4, 0 );
    // Ставим то, что было выше, на то место, где была пустышка.
    result.setAt( nullPlace, getAt( nullPlace - 4 ) );
    result.nullPlace -= 4;
    return result;
}

CPosition CPosition::Down() const
{
    assert( nullPlace <= 12 );
    
    CPosition result( *this );
    
    // Ставим пустышку ниже.
    result.setAt( nullPlace + 4, 0 );
    // Ставим то, что было ниже, на то место, где была пустышка.
    result.setAt( nullPlace, getAt( nullPlace + 4 ) );
    result.nullPlace += 4;
    return result;
}

CPosition CPosition::Left() const
{
    
    CPosition result( *this );
    
    // Ставим пустышку ниже.
    result.setAt( nullPlace - 1, 0 );
    // Ставим то, что было ниже, на то место, где была пустышка.
    result.setAt( nullPlace, getAt( nullPlace - 1 ) );
    result.nullPlace -= 1;
    return result;
}

CPosition CPosition::Right() const
{
    
    CPosition result( *this );
    
    // Ставим пустышку ниже.
    result.setAt( nullPlace + 1, 0 );
    // Ставим то, что было ниже, на то место, где была пустышка.
    result.setAt( nullPlace, getAt( nullPlace + 1 ) );
    result.nullPlace += 1;
    return result;
}

bool CPosition::isRight(CPosition self){
    if (self.data == 0x0FEDCBA987654321){
        return true;
    }
    return false;
}


int main(int argc, const char * argv[]) {
    int a;
    std::string s = "";
    std::vector<int> A(16, 0);
    for(int i = 0; i < 16; ++i){
        std::cin >> a;
        A[i] = a;
        s += std::to_string(a) + " ";
    }
    CPosition pos(s);
    
    int inv = 0;
    for (int i=0; i<16; ++i)
        if (A[i])
            for (int j=0; j<i; ++j)
                if (A[j] > A[i])
                    ++inv;
    for (int i=0; i<16; ++i)
        if (A[i] == 0)
            inv += 1 + i / 4;
    
    if ((inv & 1) == 1){
        std::cout << -1;
        return 0;
    }
    std::queue<std::pair<CPosition, std::string> > q;
    std::string w("");
    q.push(std::make_pair(pos, w));
    while(!q.empty()){
        std::pair<CPosition, std::string> curr = q.front();
        q.pop();
        if(curr.first.isRight(curr.first)){
            std::cout << curr.second.size() << '\n' << curr.second << '\n';
            break;
        }
        if(curr.first.getNullPlace() >= 4 && curr.second[curr.second.size()] != 'D'){
            std::string w = curr.second;
            w.append("U");
            q.push(std::make_pair(curr.first.Up(), w));
        }
        if( curr.first.getNullPlace() <= 12 && curr.second[curr.second.size()] != 'U'){
            std::string w = curr.second;
            w.append("D");
            q.push(std::make_pair(curr.first.Down(), w));
        }
        if(  !(curr.first.getNullPlace() == 4 || curr.first.getNullPlace() == 8 || curr.first.getNullPlace() == 12 || curr.first.getNullPlace() == 16) && curr.second[curr.second.size()] != 'L'){
            std::string w = curr.second;
            w.append("R");
            q.push(std::make_pair(curr.first.Right(), w));
        }
        if( !(curr.first.getNullPlace() == 1 || curr.first.getNullPlace() == 5 || curr.first.getNullPlace() == 9 || curr.first.getNullPlace() == 13) && curr.second[curr.second.size()] != 'R' ){
            std::string w = curr.second;
            w.append("L");
            q.push(std::make_pair(curr.first.Left(), w));
        }
    }
    return 0;
}
