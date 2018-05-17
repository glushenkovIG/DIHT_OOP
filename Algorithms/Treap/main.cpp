//
//  main.cpp
//  MyTreap
//
//  Created by Ivan on 05.11.16.
//  Copyright © 2016 Ivan. All rights reserved.
//

#include <iostream>
struct CNode {
    int Value;
    int Prior;
    
    CNode* Small;
    CNode* Big;
    
    CNode(int val, int prior);
};

CNode::CNode(int val, int prior)
    : Prior(prior),
    Value(val),
    Small(0),
    Big(0)
{
}


class CTreap {
public:
    CNode* root;
    void Split(int key);
    void Add(int key, int prior);
    void Delete(int key);
    CTreap();
    ~CTreap();

private:
    
    void split(CNode* node, int key, CNode*& Small, CNode*& Big);
    void merge(CNode* left, CNode* right);
    void add(int key, int prior, CNode*& root, CNode*& father);
    void deleteTreap(CNode*& root);
};

CTreap::CTreap(){
    root = 0;
}

CTreap::~CTreap(){
    deleteTreap(root);
}

void CTreap::deleteTreap(CNode*& node){
    if(node == 0){
        return;
    }
    deleteTreap(node->Small);
    deleteTreap(node->Big);
    delete node;
}

void CTreap::Split(int key){
    split(root, key, root->Small, root->Big);
}

CNode* Merge(CNode* left, CNode* right){
    if(left == 0){
        return right;
    }
    if(right == 0){
        return left;
    }
    else if(left->Prior < right->Prior){
        right->Small = Merge(right->Small, left);
        return right;
    }
    else {
        left->Big = Merge(left->Big, right);
        return left;
    }
}

void CTreap::split(CNode* node, int key, CNode*& left, CNode*& right){
    if(node == 0){
        left = 0;
        right = 0;
    }
    else if(node->Value <= key){
        split(node->Big, key, node->Big, right);
        left = node;
    }
    else{
        split(node->Small, key, left, node->Small);
        right = node;
    }

}

void CTreap::add(int key, int prior, CNode*& node, CNode*& father){
    if(node == 0){
        node = new CNode(key, prior);
        return;
    }
    if (node->Prior < prior){
        CNode* t2;
        t2 = node->Big;
        split(node, key, node, t2);
        father->Small = node;
        father->Big = t2;
    }
    else if(node->Value < key){
        add(key, prior, node->Big, node);
    }
    else {
        add(key, prior, node->Small, node);
    }
    
}

void CTreap::Add(int key, int prior){
    add(key, prior, root, root);
    
}

void CTreap::Delete(int key){
    CNode* node;
    node = root;
    CNode* father = root;
    int a = 0;

    while(node->Value != key){
        father = node;
        if(key > node->Value){
            node = node->Big;
            a = 1;
        }
        else{
            node = node->Small;
            a = 0;
        }
    }
    Merge(node->Small, node->Big);
    if(a == 0){
        father->Small = node->Small;
    }
    else{
        father->Big = node->Small;
    }

}

int main() {
    int n = 0;
    std::cin >> n;
    CTreap treap;
    for(int i = 0; i < n; ++i){
        int x, y;
        std::cin >> x >> y;
        treap.Add(x, y);
    }
    std::cout << treap.root->Value << treap.root->Small->Value << treap.root->Big->Value;
    return 0;
}
