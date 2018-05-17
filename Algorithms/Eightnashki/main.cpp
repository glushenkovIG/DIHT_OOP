//  main.cpp
//  C_Eightnashki
//
//  Created by Ivan on 10.03.17.
//  Copyright © 2017 Ivan. All rights reserved.
//
#include "iostream"
//#include <assert.h>
#include <sstream>
#include <string>
#include "queue"
#include "math.h"
//#include "stdafx.h"
#include <fstream>
#include <unordered_map>
class CPosition {
public:
    explicit CPosition( const std::string& source );
    
    CPosition Up() const;
    CPosition Down() const;
    CPosition Right() const;
    CPosition Left() const;
    bool isRight(CPosition self);
    int getNullPlace();
    void setAt( int place, int value );
    int data;
    
private:
    int nullPlace;
    
    int getAt( int place ) const;
};

int CPosition::getNullPlace(){
    return nullPlace;
}

CPosition::CPosition( const std::string& source_ ) :
data( 0 ),
nullPlace( 0 )
{
    std::string source;
    if (source_ == "87654321"){
         source = "087654321";
    }
    else{
        source = source_;
    }
    for( char i = 0; i < source.size(); ++i ) {
        int value = static_cast<int>( source[i] - '0');
        setAt( i,  value);
        if( value == 0 ) {
            nullPlace = i;
        }
    }
}

// Установка значения в некоторую позицию.
void CPosition::setAt( int place, int value )
{
    long long ten_place = pow(10, place);
    
    data = ((data / (ten_place * 10)) * ten_place * 10 + value  * ten_place + int(data) % ten_place);
}

// Получение того, что лежит в некоторой позиции.
int CPosition::getAt( int place ) const
{
    long long ten_place = pow(10, place);
    
    return  ( data ) % (ten_place * 10) / ten_place;
}

CPosition CPosition::Up() const
{
    
    CPosition result( *this );

    // Ставим пустышку выше.
    result.setAt( nullPlace - 3, 0 );
    // Ставим то, что было выше, на то место, где была пустышка.
    result.setAt( nullPlace, getAt( nullPlace - 3 ) );

    result.nullPlace -= 3;
    return result;
}

CPosition CPosition::Down() const
{
    CPosition result( *this );

    // Ставим пустышку ниже.
    result.setAt( nullPlace + 3, 0 );
    // Ставим то, что было ниже, на то место, где была пустышка.
    result.setAt( nullPlace, getAt( nullPlace + 3 ) );

    result.nullPlace += 3;
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
    if (self.data == 87654321){
        return true;
    }
    return false;
}


int main(int argc, const char * argv[]) {
    
    std::ifstream fin("puzzle.in");
    std::ofstream fout("puzzle.out");
    std::unordered_map<int, std::pair<int, char> > H;
    //H.emplace(1, std::make_pair(1, 'm'));
    std::queue<CPosition> Q;
    int going = 87654321;
    Q.push(CPosition(std::to_string(going)));
    CPosition b(std::to_string(going));
    std::pair<int, char> isEmpty = std::make_pair(int(0), char(0));
    CPosition curr = Q.front();
    
    int a = 0;
    std::string s = "";
    std::vector<int> A(9, 0);
    for(int i = 0; i < 9; ++i){
        fin >> a;
        A[i] = a;
        s += std::to_string(a);
    }
    CPosition pos(s);
    int c = pos.data;

    while((!Q.empty()) && (c != curr.data)){
        curr = Q.front();
        Q.pop();
        if (curr.getNullPlace() >= 3 && H[curr.Up().data] == isEmpty){
            CPosition pos = curr.Up();
            Q.push(pos);
            H[pos.data] = std::make_pair(curr.data, 'D');
        }
        if (curr.getNullPlace() <= 5 && H[curr.Down().data] == isEmpty){
            CPosition pos = curr.Down();
            Q.push(pos);
            H[pos.data] = std::make_pair(curr.data, 'U');
        }
        if (!(curr.getNullPlace() == 0 || curr.getNullPlace() == 3 || curr.getNullPlace() == 6) && H[curr.Left().data] == isEmpty){
            CPosition pos = curr.Left();
            Q.push(pos);
            H[pos.data] = std::make_pair(curr.data, 'R');
        }
        if (!(curr.getNullPlace() == 2 || curr.getNullPlace() == 5 || curr.getNullPlace() == 8) && H[curr.Right().data] == isEmpty){
            CPosition pos = curr.Right();
            Q.push(pos);
            H[pos.data] = std::make_pair(curr.data, 'L');
        }
        
    }
    while(c != going){
        char a = H[c].second;
        fout << a;
        c = H[c].first;
    }
    /*
    std::queue<std::pair<CPosition, std::string> > q;
    std::string w("");
    q.push(std::make_pair(pos, w));
    while(!q.empty()){
        std::pair<CPosition, std::string> curr = q.front();
        q.pop();
        if(curr.first.isRight(curr.first)){
            fout << curr.second.size() << '\n' << curr.second << '\n';
            break;
        }
        if(curr.first.getNullPlace() >= 3 && curr.second[curr.second.size()] != 'D'){
            std::string w = curr.second;
            w.append("U");
            q.push(std::make_pair(curr.first.Up(), w));
        }
        if( curr.first.getNullPlace() <= 5 && curr.second[curr.second.size()] != 'U'){
            std::string w = curr.second;
            w.append("D");
            q.push(std::make_pair(curr.first.Down(), w));
        }
        if(  !(curr.first.getNullPlace() == 2 || curr.first.getNullPlace() == 5 || curr.first.getNullPlace() == 8) && curr.second[curr.second.size()] != 'L'){
            std::string w = curr.second;
            w.append("R");
            q.push(std::make_pair(curr.first.Right(), w));
        }
        if( !(curr.first.getNullPlace() == 0 || curr.first.getNullPlace() == 3 || curr.first.getNullPlace() == 6) && curr.second[curr.second.size()] != 'R' ){
            std::string w = curr.second;
            w.append("L");
            q.push(std::make_pair(curr.first.Left(), w));
        }
    }
    fout.close();
    fin.close();
    return 0;
     */
}


