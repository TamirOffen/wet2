#ifndef UNIONFIND_H
#define UNIONFIND_H

#include <iostream>
#include <stdio.h>
using namespace std;


// template<class T>
struct group_uf;

// template <class T>
struct node_uf
{
    // T key;
    // node<T> * father;

    node_uf() {};

    int key;
    node_uf * father;
};

// template <class T>
struct tree_uf
{
    // node<T> * root;
    // group<T> * group;

    tree_uf() : root(nullptr), group(nullptr) {};

    node_uf * root;
    group_uf * group;
};

// template <class T>
struct group_uf
{
    int size=1;
    // tree<T> * tree;
    // T groupID;
    int groupID;
    tree_uf * tree;

    group_uf() {tree=nullptr;};
};

// template <class T>
class UnionFind
{   
    /*
    public:
    
    group<T> * makeSet(int i);
    UnionFind(int n);
    ~UnionFind();
    T union_groups(T group1, T group2);
    T find(T i);


    int size;
    node<T> ** items;
    group<T> ** groups;
    
    */
    
    public:
    void makeSet(int i);
    UnionFind(int size);
    ~UnionFind();
    int union_groups(int group1, int group2);
    int find(int i);


    
    node_uf ** items = nullptr;
    group_uf ** groups = nullptr;
    int size;
    
};

/*
template<class T>
UnionFind<T>::UnionFind(int n)
{
    size = n;
    *items = (node<T>*)malloc(sizeof(node<T>*)*size);
    *groups = (group<T>*)malloc(sizeof(group<T>*)*size);
    for(int i=0; i<size; i++)
    {
        groups[i] = makeSet(i);
        items[i] = groups[i]->tree->root;
    }
}

template<class T>
UnionFind<T>::~UnionFind()
{   
    for(int i=0; i<size; i++)
        delete items[i];
    for(int i=0; i<size; i++)
    {
        if(groups[i]->groupID == i)
            delete groups[i]->tree;
        delete groups[i]; 
    }
    free(items);
    free(groups);
}

//only helps to initialize the DS
template<class T>
group<T> * UnionFind<T>::makeSet(int i)
{
    group<T> * g = new group<T>();
    g->groupID = i;
    g->size = 1;
    tree<T> * t = new tree<T>();
    node<T> * n = new node<T>();
    n->father = nullptr;
    n->key = i;
    t->group = g;
    t->root = n;
    g->tree = t;
    return g;
}

//returns the groupID of the unioned group - it will be the groupID of the bigger group
// if both group have equal size so return groupID of group1
template<class T>
T UnionFind<T>::union_groups(T group1, T group2)
{
    if(groups[group1]->size >= groups[group2]->size)
    {
        groups[group2]->tree->root->father = groups[group1]->tree->root;
        delete groups[group2]->tree;
        groups[group1]->size += groups[group2]->size;
        //groups[group2]->tree = groups[group1]->tree;
        //groups[group2]->groupID = group1;
        groups[group2] = groups[group1];
        return group1;
    }
    groups[group1]->tree->root->father = groups[group2]->tree->root;
    delete groups[group1]->tree;
    groups[group2]->size += groups[group1]->size;
    groups[group1] = groups[group2];
    return group2;
}


//returns groupID
template<class T>
T UnionFind<T>::find(T i)
{
    node<T> * n = items[i];
    while(n->father)
        n=n->father;
    return n->key;
}

*/

#endif