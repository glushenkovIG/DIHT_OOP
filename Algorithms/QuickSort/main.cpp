//
//  main.cpp
//  quickSort
//
//  Created by Ivan on 28.10.16.
//  Copyright © 2016 Ivan. All rights reserved.
//
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <time.h>
#include <string>
#include <vector>
#include <memory.h>

int Partition(int* A, int l, int r){
    int i , j, num_piv;
    i = l;
    j = l;
    num_piv = (r + l) / 2;
    std::swap(A[r - 1], A[num_piv]);
    
    while(j <= r - 1){
        if(A[j] > A[r - 1]) j++;
        else{
            std::swap(A[i], A[j]);
            i++;
            j++;
        }
        
        /*for(int i = 0; i < n; i++){
         std::cout << A[i] << " ";
         }
         std::cout << "(" << i << j << ")" << "\n";*/
        
    }
    std::swap(A[r - 1], A[i]);
    std::cout << "(";
    for( int i = l; i < r; ++i) {
        std::cout << A[i] << " ";
    }

    return i;
    
}



void quickSort(int* A, int l, int r){
    if(l == r){
        return;
    }
    int i = 0;
    int k = 0;
    i = Partition(A, l, r);

    k = Partition(A, l, i);
    k = Partition(A, i + 1, r);
    
    quickSort(A, l, (l + r) / 2);
    quickSort(A, (l + r) / 2 + 1, r);

}
int main()
{
    std::vector<int> A;
    while(true){
        int value;
        std::cin >> value;
        if(std::cin.fail() || std::cin.eof()){
            break;
        }
        A.push_back(value);
    }
    int* B = new int[6];
    
    
    for( int i = 0; i < A.size(); ++i) {
        B[i] = A[i];
    }
    quickSort(B, 0, 6);
    for( int i = 0; i < A.size(); ++i) {
        std::cout << B[i] << " ";
    }
    return 0;
}

























