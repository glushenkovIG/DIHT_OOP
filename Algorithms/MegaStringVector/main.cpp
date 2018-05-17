//
//  main.cpp
//  4_StringVector
//
//  Created by Ivan on 19.05.17.
//  Copyright © 2017 Ivan. All rights reserved.
//

#include <iostream>
#include <memory.h>
#include <queue>
#include <stack>
#include <assert.h>
#include <vector>

struct CBNode{
    unsigned char H;
    int count;//количество элементов в левом поддереве плюс количество элементов в правом поддереве + 1
    std::string String;
    CBNode* Small;
    CBNode* Big;
    CBNode() : Small(0), Big(0), H(1), String(""), count(1){}
    CBNode(std::string& str) : Small(0), Big(0), H(1), String(str), count(1){}
};

class AvlTree {
public:
    AvlTree() : root(0) {}
    ~AvlTree();
    
    void Add(int num, std::string& str);
    void Del(int key);
    
    unsigned char High(CBNode* node);
    int Count(CBNode* node);

    
    std::string GetAt(int key);
    
    void get_elem(CBNode* node, int key, CBNode*& ans);
    CBNode* move(CBNode* node);
    CBNode* l_move(CBNode* node);
    CBNode* r_move(CBNode* node);
    CBNode* deleteElem(CBNode* node, int key);
    CBNode* minNode(CBNode*);
    CBNode* root;
    CBNode* delMin(CBNode* node);
    
private:
    void deleteNode(CBNode* node);
    CBNode* add(CBNode*& node, int num, std::string& str);
    int balance(CBNode* node);
    void trueHigh(CBNode* node);
};

void AvlTree::Add(int num, std::string& str){
    root = add(root, num, str);
}

CBNode* AvlTree::add(CBNode*& node, int num, std::string& str){
    if(node == 0){
        return new CBNode(str);
    }
    else if(Count(node->Small) == num){
        CBNode* now = new CBNode(str);
        now->Small = node->Small;
        now->count += Count(node->Small);
        trueHigh(now);
        node->count += 1;
        node->Small = now;
        trueHigh(node);
        return move(node);
    }
    else if (num > Count(node->Small)){
        node->count += 1;
        node->Big = add(node->Big, num - Count(node->Small) - 1, str);
    }
    else if(num < Count(node->Small) ){
        node->count += 1;
        node->Small = add(node->Small, num, str);
    }
    return move(node);
}

std::string AvlTree::GetAt(int key){
    CBNode* ans = new CBNode();
    get_elem(root, key, ans);
    return ans->String;
}

void AvlTree::get_elem(CBNode* node, int num, CBNode*& ans){
    if(node == 0){
        return;
    }
    if(Count(node->Small) == num){
        ans = node;
        return;
    }
    else if(Count(node->Small) < num){
        ans = node;
        get_elem(node->Big, num - Count(node->Small) - 1, ans);
    }
    else if(Count(node->Small) > num){
        ans = node;
        get_elem(node->Small, num, ans);
    }
    return;
}

void AvlTree::Del(int num){
    root = deleteElem(root, num);
}

CBNode* AvlTree::deleteElem(CBNode* node, int num){
    if(node == 0){
        return 0;
    }
    else if(Count(node->Small) == num){
        if(node->Big != NULL){
            node = l_move(node);
            node->Small = node->Small->Small;
            
            trueHigh(node);
        }
        else{
            return node->Small;
        }
        return move(node);
    }
    else if(Count(node->Small) < num){
        node->count -= 1;
        node->Big = move(deleteElem(node->Big, num - Count(node->Small) - 1));
    }
    else if(Count(node->Small) > num){
        node->count -= 1;
        node->Small = move(deleteElem(node->Small, num));
    }
    return move(node);
}

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

int AvlTree::Count(CBNode* node){
    if(node == 0){
        return 0;
    }
    return node->count;
}

void AvlTree::trueHigh(CBNode* node){
    node->H = std::max(High(node->Small), High(node->Big)) + 1;
    node->count = Count(node->Small) + Count(node->Big) + 1;
}

int AvlTree::balance(CBNode* node){
    return High(node->Small) - High(node->Big);
}

CBNode* AvlTree::move(CBNode* node){
    if(node == 0){
        return 0;
    }
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


int main(){
    int n = 0;
    std::cin >> n;
    
    AvlTree tree;
    int key;
    char a;
    std::string str;
    std::queue<std::string> Q;
    
    for(int i = 0; i < n; ++i){
        std::cin >> a >> key;
        if(a == '+'){
            std::cin >> str;
            tree.Add(key, str);
        }
        else if(a == '-'){
            tree.Del(key);
        }
        else{
            Q.push(tree.GetAt(key));
        }
    }
    
    while(!Q.empty()){
        std::cout << Q.front() << '\n';
        Q.pop();
    }
    return 0;
}
