//
//  main.cpp
//  AvlTree
//
//  Created by Ivan on 07.11.16.
//  Copyright © 2016 Ivan. All rights reserved.

#include <iostream>
#include <memory.h>
#include <queue>
#include <stack>
#include <assert.h>
#include <vector>

struct CBNode{
    int Value;
    unsigned char H;
    CBNode* Small;
    CBNode* Big;
    CBNode(int val) : Value(val), Small(0), Big(0), H(1){}
};

class AvlTree {
public:
    AvlTree() : root(0) {}
    ~AvlTree();
    void Add(int elem);
    void levelOrder();
    unsigned char High(CBNode* node);
    void Del(int key);
    CBNode* move(CBNode* node);
    CBNode* l_move(CBNode* node);
    CBNode* r_move(CBNode* node);
    CBNode* deleteElem(CBNode* node, int key);
    CBNode* minNode(CBNode*);
    CBNode* root;
    CBNode* delMin(CBNode* node);
    
private:
    void deleteNode(CBNode* node);
    CBNode* add(CBNode*& node, int elem);
    int balance(CBNode* node);
    void trueHigh(CBNode* node);
};

AvlTree::~AvlTree(){
    deleteNode(root);
}

void AvlTree::deleteNode(CBNode* node_){
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

unsigned char AvlTree::High(CBNode* node){
    if(node == 0){
        return 0;
    }
    return node->H;
}

int AvlTree::balance(CBNode* node){
    return High(node->Small) - High(node->Big);
}

void AvlTree::trueHigh(CBNode* node){
    node->H = std::max(High(node->Small), High(node->Big)) + 1;
}

CBNode* AvlTree::move(CBNode* node){
    trueHigh(node);
    if (balance(node) == -2){
        if(node->Big != 0 && balance(node->Big) >= 0){
            node->Big = r_move(node->Big);
        }
        return l_move(node);
    }
    if (balance(node) == 2){
        if(node->Small != 0 && balance(node->Small) <= 0){
            node->Small = l_move(node->Small);
        }
        return r_move(node);
    }
    return node;
}

CBNode* AvlTree::r_move(CBNode* node){
    CBNode* now = node->Small;
    node->Small = now->Big;
    now->Big = node;
    trueHigh(node);
    trueHigh(now);
    return now;
}

CBNode* AvlTree::l_move(CBNode* node){
    CBNode* now = node->Big;
    node->Big = now->Small;
    now->Small = node;
    trueHigh(node);
    trueHigh(now);
    return now;
}

void AvlTree::Add(int elem){
    root = add(root, elem);
}

CBNode* AvlTree::add(CBNode*& node, int elem){
    if(node == 0){
        return new CBNode(elem);
    }
    if (elem > node->Value){
        node->Big = add(node->Big, elem);
    }
    else{
        node->Small = add(node->Small, elem);
    }
    return move(node);
}

CBNode* AvlTree::minNode(CBNode* node){
    if (node->Small != 0){
        return minNode(node->Small);
    }
    else{
        return node;
    }
}

CBNode* AvlTree::delMin(CBNode* node){
    if (node->Small == 0){
        return node->Big;
    }
    node->Small = delMin(node->Small);
    return move(node);
}

CBNode* AvlTree::deleteElem(CBNode* node, int key){
    if(node == 0){
        return 0;
    }
    else if(node->Value < key){
        node->Big = deleteElem(node->Big, key);
    }
    else if(node->Value > key){
        node->Small = deleteElem(node->Small, key);
    }
    else{
        CBNode* r = node->Big;
        CBNode* l = node->Small;
        delete node;
        if(r == 0){
            return l;
        }
        else if (l == 0){
            return r;
        }
        
        CBNode* m = minNode(r);
        m->Big = delMin(m);
        m->Small = l;
        return move(m);
    }
    return move(node);
}

void AvlTree::Del(int key){
    root = deleteElem(root, key);
}

void AvlTree::levelOrder(){
    std::queue <CBNode*> myQ;
    myQ.push(root);
    while( !myQ.empty() ) {
        CBNode* node = myQ.front();
        myQ.pop();
        if( node == 0 ) {
            continue;
        }
        std::cout << "(" << node->Value << ", " << int(node->H) << ")" << " ";
        myQ.push( node->Small );
        myQ.push( node->Big );
    }
}

int main(){
    int n = 0;
    std::cin >> n;
    AvlTree tree;
    for(int i = 0; i < n; ++i){
        int key;
        std::cin >> key;
        tree.Add(key);
    }
    tree.Del(int(2));
    tree.levelOrder();
    return 0;
}





























