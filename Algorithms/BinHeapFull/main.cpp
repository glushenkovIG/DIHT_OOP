//
//  main.cpp
//  binHeap
//
//  Created by Ivan on 31.10.16.
//  Copyright © 2016 Ivan. All rights reserved.
//

#include <iostream>
#include <memory.h>
#include <queue>
#include <stack>
#include <assert.h>
#include <vector>

struct CBNode{
    int Value;
    CBNode* Small;
    CBNode* Big;
    CBNode(int val) : Value(val), Small(0), Big(0) {}
};

class BinTree {
public:
    BinTree() : root(0) {}
    ~BinTree();
    void Add(int elem);
    void PostOrder(std::vector<int>&);
    void PreOrder();
    void InOrder();
    void LevelOrder();
    void levelOrder();
    int High();
private:
    CBNode* root;
    void deleteNode(CBNode* node);
    void add(CBNode*& node, int elem);
    void postOrder(CBNode* node);
    void preOrder(CBNode* node);
    void inOrder(CBNode* node);
};

BinTree::~BinTree(){
    deleteNode(root);
}

void BinTree::Add(int elem){
    if(root == 0){
        root = new CBNode(elem);
        return;
    }
    else{
        CBNode* father;
        CBNode* now;
        father = 0;
        now = root;
        while(now != 0){
            father = now;
            if(elem < now->Value){
                now = now->Small;
            }
            else{
                now = now->Big;
            }
        }
        assert(father != 0);
        if(elem < father->Value){
            father->Small = new CBNode(elem);
        }
        else{
            father->Big = new CBNode(elem);
        }
        
    }
}

void BinTree::PostOrder(std::vector<int>& A){
    std::stack <CBNode*> st;
    CBNode* node = root;
    CBNode* now = 0;
    CBNode* last = 0;
    while(st.size() != 0 || node != 0){
        if(node != 0){
            st.push(node);
            node = node->Small;
        }
        else{
            now = st.top();
            if(now->Big != 0 && last != now->Big){
                node = now->Big;
            }
            else{
                st.pop();
                A.insert(A.end(), now->Value);
                last = now;
            }
        }
    }
}

void BinTree::PreOrder(){
    preOrder(root);
}

void BinTree::InOrder(){
    inOrder(root);
}

void BinTree::deleteNode(CBNode* node_){
    if(node_ == 0){
        return;
    }
    std::stack <CBNode*> st;
    CBNode* node = node_;
    CBNode* now = 0;
    CBNode* last = 0;
    int h = 0;
    while(st.size() != 0 || node != 0){
        if(node != 0){
            st.push(node);
            node = node->Small;
            ++h;
        }
        else{
            now = st.top();
            if(now->Big != 0 && last != now->Big){
                node = now->Big;
            }
            else{
                st.pop();
                --h;
                delete now;
                last = now;
            }
        }
    }


}

void BinTree::preOrder(CBNode* node){
    if(node == 0){
        return;
    }
    std::cout << node->Value << " ";
    preOrder(node->Small);
    preOrder(node->Big);
}

void BinTree::inOrder(CBNode* node){
    if(node == 0){
        return;
    }
    inOrder(node->Small);
    std::cout << node->Value << " ";
    inOrder(node->Big);

}

void BinTree::levelOrder(){
    std::queue <CBNode*> myQ;
    myQ.push(root);
    while( !myQ.empty() ) {
        CBNode* node = myQ.front();
        myQ.pop();
        if( node == 0 ) {
            continue;
        }
        std::cout << node->Value << " ";
        myQ.push( node->Small );
        myQ.push( node->Big );
    }

    
}

int BinTree::High(){
    std::stack <CBNode*> st;
    CBNode* node = root;
    CBNode* now = 0;
    CBNode* last = 0;
    int max_ = 0;
    int h = 0;
    while(st.size() != 0 || node != 0){
        if(node != 0){
            st.push(node);
            node = node->Small;
            ++h;
        }
        else{
            now = st.top();
            if(now->Big != 0 && last != now->Big){
                node = now->Big;
            }
            else{
                st.pop();
                --h;
                last = now;
            }
        }
        max_ = std::max(h, max_);
    }
    return max_;
}


int main() {
    int n = 0;
    std::cin >> n;
    BinTree tree;
    for ( int i = 0; i < n; ++i) {
        int elem;
        std::cin >> elem;
        tree.Add(elem);
    }
    //std::cout << tree.root->Value;
    std::vector<int> A;

    tree.PostOrder(A);
    for (int i = 0; i < A.size(); ++i) {
        std::cout << A[i] << ' ';
    }
    return 0;
}
