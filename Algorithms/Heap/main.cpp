//
//  main.cpp
//  hashTable
//
//  Created by Ivan on 06.12.16.
//  Copyright © 2016 Ivan. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <typeinfo>

long long H(std::string& A, int size){
    long long ans = 0;
    for(int i = 0; i < A.size(); ++i){
        ans = (ans + (int)A[i]) * 41;
        ans = ans % size;
    }
    return ans;
}

long long h2(std::string& A, int size_){
    long long ans = 0;
    for(int i = 0; i < A.size(); ++i){
        ans = (ans + (int)A[i]) * 131;
        ans = ans % size_;
    }
    return (ans * 2 + 1) % size_;
}

long long H(int& A, int size){
    long long ans = 0;
    for(int i = 0; i < A; ++i){
        ans = (ans + (int)A % 10) * 41;
        ans = ans % size;
        A = A / 10;
    }
    return ans;
}

long long h2(int& A, int size_){
    long long ans = 0;
    for(int i = 0; i < A; ++i){
        ans = (ans + (int)A % 10) * 131;
        ans = ans % size_;
        A = A / 10;
    }
    return (ans * 2 + 1) % size_;
}


template <class T>
class HashTable{
public:
    bool Add(T& A);
    bool Del(T& A);
    bool isIn(T& A);
    void makeBigger();
    HashTable(int s, const T& n, const T& d) : size(s), table(s, n), nill(n), del(d) {}
    ~HashTable(){}
private:
    int elemsCount = 0;
    std::vector<T> table;
    int s = 8;
    int size = 8;
    T del;
    T nill;
};

template <class T>
void HashTable<T>::makeBigger(){
//    HashTable table1(size * 2, nill, del);
//    for(int i = 0; i < size; ++i){
//        table1.Add(table[i]);
//    }
//    size *= 2;
//    table = table1.table;
    std::vector<T> new_table(size * 2, nill);
    bool end = false;

    for (int i = 0; i < size; ++i) {
        long long h = H(table[i], size * 2);
        long long H2 = h2(table[i], size * 2);
        end = false;
        for(int k = 0; k < size * 2 && !end; ++k){
            int j = (h + (k * H2) % (size * 2)) % (size * 2);
            if(new_table[j] == nill){
                new_table[j] = table[i];
                end = true;
                break;
    
            }
        }
    }
    
    table = new_table;
    size *= 2;
}

template <class T>
bool HashTable<T>::Add(T& A){
    if((elemsCount + 1.0) / size >= 0.66){
        makeBigger();
    }
    bool wasDEL = false;
    int numDEL = 0;
    long long h = H(A, size);
    long long H2 = h2(A, size);
    for (int i = 0; i < size; ++i) {
        int j = (h + (i * H2) % size) % size;
        if(table[j] == nill){
            if(wasDEL){
                table[numDEL] = A;
                ++elemsCount;
                return true;
            }
            table[j] = A;
            ++elemsCount;
            return true;
        }
        if (table[j] == A) return false;
        if(table[j] == del && !wasDEL){
            wasDEL = true;
            numDEL = j;
        }
    }
    if(wasDEL){
        table[numDEL] = A;
        ++elemsCount;
        return true;
    }

    return false;
}

template <class T>
bool HashTable<T>::Del(T& A){
    long long h = H(A, size);
    long long H2 = h2(A, size);
    for (int i = 0; i < size; ++i) {
        int j = (h + (i * H2) % size) % size;
        if(table[j] == A){
            table[j] = del;
            --elemsCount;
            return true;
        }
        if(nill == table[j]){
            return false;
        }
    }
    return false;
}

template <class T>
bool HashTable<T>::isIn(T& A){
    long long h = H(A, size);
    long long H2 = h2(A, size);
    for (int i = 0; i < size; ++i) {
        int j = (h + (i * H2) % size) % size;
        if (table[j] == A){
            return true;
        }
        if(nill == table[j]){
            return false;
        }
    }
    return false;
}

int main(int argc, const char * argv[]) {
    HashTable<int> A( 4, -1, -2);
    char operation;
    int word;
    while( std::cin >> operation >> word ) {
        switch( operation ) {
            case '+':
                std::cout << (A.Add( word ) ? "OK" : "FAIL");
                std::cout << '\n';
                break;
            case '-':
                std::cout << (A.Del( word ) ? "OK" : "FAIL");
                std::cout << '\n';
                break;
            case '?':
                std::cout << (A.isIn( word ) ? "OK" : "FAIL");
                std::cout << '\n';
                break;
            default:
                assert( false );
        }
    }
    
    return 0;
}
