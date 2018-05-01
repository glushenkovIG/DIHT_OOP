//
//  main.cpp
//  4_aho_korasik
//
//  Created by Ivan on 30.09.17.
//  Copyright © 2017 Ivan. All rights reserved.
//

#include <iostream>
#include <cstring>
#include <vector>
#include <unordered_map>

std::unordered_map<std::string, int> S;


const int k = 26, NMAX = 10000;
struct CNode{
    int next_node[k];
    int pat_num;
    int suff_link;
    int auto_change[k];
    int par;
    int good_suff_link;
    bool flag;
    char symb;
};

class CMyclass{
public:
    
    CMyclass();
    void add_pattern( const std::string& s );
    void find_all_pos( const std::string& s );
    //std::vector<int> A;

private:
    
    void check( int v, int i );

    CNode make_node( int p, char c );
    
    bool is_string_in_bohr( const std::string& s );
    
    int get_auto_change( int v, char ch );
    int get_suff_link( int v );
    int get_good_suff_link( int v );
    
    std::vector<std::string> pattern;
    std::vector<CNode> bor;
    
    
};

std::vector<std::string> pattern;
std::vector<int> C;
std::vector<CNode> bor;


CMyclass::CMyclass(){
    bor.push_back(make_node( 0, '$' ));
}

CNode CMyclass::make_node(int p, char c){
    CNode v;
    memset(v.next_node, 255, sizeof( v.next_node ));
    memset(v.auto_change, 255, sizeof( v.auto_change ));
    v.flag = false;
    v.suff_link = -1;
    v.par = p;
    v.symb = c;
    v.good_suff_link = -1;
    return v;
}


void CMyclass::add_pattern( const std::string& s ){
    int num = 0;
    for (int i = 0; i < s.length(); ++i){
        char ch = s[i] - 'a';
        if(bor[num].next_node[ch] == -1){
            bor.push_back(make_node(num, ch));
            bor[num].next_node[ch] = bor.size() - 1;
        }
        num = bor[num].next_node[ch];
    }
    bor[num].flag = true;
    pattern.push_back(s);
    bor[num].pat_num = pattern.size() - 1;
}

bool CMyclass::is_string_in_bohr( const std::string& s ){
    int num = 0;
    for(int i = 0; i < s.length(); ++i){
        char ch = s[i] - 'a';
        if( bor[num].next_node[ch] == -1 ){
            return false;
        }
        num = bor[num].next_node[ch];
    }
    return true;
}

int CMyclass::get_suff_link( int v ){
    if( bor[v].suff_link == -1 ){
        if( v == 0 || bor[v].par == 0 ){
            bor[v].suff_link = 0;
        }
        else{
            bor[v].suff_link = get_auto_change(get_suff_link(bor[v].par), bor[v].symb);
        }
    }
    return bor[v].suff_link;
}

int CMyclass::get_auto_change( int v, char ch ){
    if(bor[v].auto_change[ch] == -1){
        if(bor[v].next_node[ch] != -1){
            bor[v].auto_change[ch] = bor[v].next_node[ch];
        }
        else{
            if( v == 0 ){
                bor[v].auto_change[ch] = 0;
            }
            else{
                bor[v].auto_change[ch] = get_auto_change(get_suff_link(v), ch);
            }
        }
    }
    return bor[v].auto_change[ch];
}

int CMyclass::get_good_suff_link( int v ){
    if( bor[v].good_suff_link == -1 ){
        int u = get_suff_link(v);
        if( u == 0 ){
            bor[v].good_suff_link = 0;
        }
        else{
            bor[v].good_suff_link = (bor[u].flag) ? u : get_good_suff_link(u);
        }
    }
    return bor[v].good_suff_link;
}

void CMyclass::check( int v, int i ){
    for(int u = v; u != 0; u = get_good_suff_link(u)){
        if( bor[u].flag ){
            if(0 <= (i - (int) pattern[bor[u].pat_num].length() - (int) S.at(pattern[bor[u].pat_num]))) {
                
                C[i - pattern[bor[u].pat_num].length() - S.at(pattern[bor[u].pat_num])] += 1;
            }
            //std::cout << i - pattern[bor[u].pat_num].length() + 1 << " " << pattern[bor[u].pat_num] << std::endl;
        }
    }
}

void CMyclass::find_all_pos( const std::string& s ){
    int u = 0;
    for(int i = 0; i < s.length(); ++i){
        u = get_auto_change(u, s[i] - 'a');
        check(u, i + 1);
    }
}

int main(){
    std::vector<std::string> patterns;
    std::string text, pattern;
    std::cin >> pattern;
    std::cin >> text;
    C.resize(text.size());
    bool flag = true;
    std::string curr_patt = "";
    CMyclass A;
    int curr_count = 0;
    for(int i = 0; i < pattern.size(); ++i){
        if(pattern[i] != '?' && flag == true){
            curr_patt = "";
            ++curr_count;
            while(pattern[i] != '?' && i < pattern.size()){
                curr_patt += pattern[i];
                ++i;
            }
            A.add_pattern(curr_patt);
            S.insert(std::make_pair(curr_patt, i - curr_patt.size()));
            --i;
        }
    }
    //std::cout << C[0];
    A.find_all_pos(text);
    for(int i = 0; i < C.size(); ++i){
        //std::cout <<'(' << C[i] << ')';

        if(C[i] == curr_count){
            std::cout << i << ' ';
        }
    }
}
