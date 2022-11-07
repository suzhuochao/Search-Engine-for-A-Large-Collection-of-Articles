
//
// Created by zhuochao su on 4/7/22.
//

#ifndef INC_22S_FINAL_PROJ_AVLTREE_H
#define INC_22S_FINAL_PROJ_AVLTREE_H
#include "iostream"
#include "fstream"
using namespace std;

#include <vector>
#include <iostream>

using namespace std;

template <typename T>
class treeNode{
public:
    T _key;
    vector<T> _values;
    treeNode* left;
    treeNode* right;
    int height;

public:
    treeNode() {
        left = nullptr;
        right = nullptr;
        height = 0;
    }
    void add_value(T value) {
        this->_values.push_back(value);
    }
    treeNode(T key, T value, treeNode* left, treeNode* right){
        this->right = right;
        this->left = left;
        this->_key = key;
        this->_values.push_back(value);
    }
};

template <typename T>
class AVLTree {
public:
    treeNode<T>* root;
    ofstream outfile;
    unsigned long node_num = 0;
public:
    AVLTree(){
        node_num = 0;
        root = nullptr;
    };
    AVLTree(const AVLTree& rhs) : root(nullptr){
        *this = rhs;
    }
    ~AVLTree() = default;

    void clear(){
        node_num = 0;
        clear(root);
    }
    void clear(treeNode<T>* tree){
        if(!tree){
            return;
        }
        clear(tree->left);
        clear(tree->right);
        delete tree;
        tree = nullptr;
    }

    unsigned long get_node_num(){
        return node_num;
    }

    void insert(T& key, T& value) {
        insert(root, key, value);
    }

    int getHeight(treeNode<T>* root){
        if(root == nullptr){
            return 0;
        }
        return max(getHeight(root->left), getHeight(root->right))+1;
    }

    int getBalance(treeNode<T>* root){
        return getHeight(root->left) - getHeight(root->right);
    }

    void rrotate(treeNode<T>*& root){
        treeNode<T>* temp = root->left;
        root->left = temp->right;
        temp->right = root;
        root = temp;
    }

    void lrotate(treeNode<T>*& root){
        treeNode<T>* temp = root->right;
        root->right = temp->left;
        temp->left = root;
        root = temp;
    }

    void insert(treeNode<T>*& t, T& key, T& value){
        if(t == nullptr){
            node_num++;
            t = new treeNode<T>(key, value, nullptr, nullptr);
            return;
        }
        else if(key < t->_key){
            insert(t->left, key, value);
        }
        else if(t->_key < key){
            insert(t->right, key, value);
        } else{
            t->add_value(value);
        }
    };

    int height(treeNode<T>*t){
        return t == nullptr?-1:t->height;
    };

    /*void balance(treeNode<T>*&t){
        if(t == nullptr){
            return;
        }
        if(height(t->left) - height(t->right) > 1){
            if(height(t->left->left) >= height(t->left->right)){
                rotateWithLeftChild(t);
            } else{
                doubleWithLeftChild(t);
            }
        } else if(height(t->right) - height(t->left) > 1){
            if(height(t->right->right) >= height(t->right->left)){
                rotateWithRightChild(t);
            } else {
                doubleWithRightChild(t);
            }
        }
        t->height = max(height(t->left), height(t-> right))+1;
    };
    treeNode<T>* rotateWithLeftChild(treeNode<T>*& k2){
        treeNode<T>* k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2->height = max(height(k2->left), height(k2->right))+1;
        k1->height = max(height(k1->left), k2->height)+1;
        k2 = k1;
    };
    treeNode<T>* rotateWithRightChild(treeNode<T>*& k2){
        treeNode<T>* k1 = k2->right;
        k2->right = k1->left;
        k1->left = k2;
        k2->height = max(height(k2->left), height(k2->right))+1;
        k1->height = max(height(k1->left), k2->height)+1;
        k2 = k1;
    };
    treeNode<T>* doubleWithLeftChild(treeNode<T>*& t){
        t->left = rotateWithLeftChild(t->left);
        return rotateWithRightChild(t->right);
    };
    treeNode<T>* doubleWithRightChild(treeNode<T>*& t){
        t->right = rotateWithRightChild(t->right);
        return rotateWithLeftChild(t->left);
    };*/

    bool find(treeNode<T>* tmp, T& key, vector<T>& value) {
        if (tmp == nullptr) {
            return false;
        }
        if (tmp->_key == key) {
            for (int i = 0; i < tmp->_values.size(); ++i) {
                value.push_back(tmp->_values[i]);
            }
            return true;
        }
        if (tmp->_key > key) {
            return find(tmp->left, key, value);
        } else {
            return find(tmp->right, key, value);
        }
    }

    bool find(T& key, vector<T>& value) {
        treeNode<T>* tmp = root;
        return find(tmp, key, value);
    }

    void save_disk(string& path){
        outfile.open(path.c_str(), ios::out | ios::trunc);
        treeNode<T>* temp = root;
        save_disk(temp);
        outfile.close();
    }
    void save_disk(treeNode<T>* tree){
        if(tree == nullptr){
            return;
        }
        string info;
        info += tree->_key;
        info += ":";
        for (int i = 0; i < tree->_values.size(); ++i) {
            info += tree->_values[i];
            info += "#";
        }
        outfile << info << endl;
        save_disk(tree->left);
        save_disk(tree->right);
    }

    void printTree(treeNode<T>* tree){
        if(tree == nullptr){
            return;
        }
        cout << tree->_key << " : ";
        for (int i = 0; i < tree->_values.size(); ++i) {
            cout << tree->_values[i] << ", ";
        }
        cout << endl;
        printTree(tree->left);
        printTree(tree->right);
    }

    void printTree() {
        return printTree(root);
    }

};

#endif //INC_22S_FINAL_PROJ_AVLTREE_H
