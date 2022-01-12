
#include <iostream>
#include <stdio.h>
#include "unionfind.h"

#include <cassert>

UnionFind::UnionFind(int size)
{   
    
    size = size;
    items = new node_uf*[size];
    groups = new group_uf*[size];
    // items[0] = nullptr;
    // groups[0] = nullptr;
    for(int i=0; i<size; i++)
        makeSet(i);
    
}

UnionFind::~UnionFind()
{   
    for(int i=0; i<size; i++) {
        if(items[i] != nullptr) delete items[i];
    }
    for(int i=0; i<size; i++)
    {
        if(groups[i]->groupID == i) {
            if(groups[i]->tree != nullptr) delete groups[i]->tree;
        }
        if(groups[i] != nullptr) delete groups[i]; 
    }
    delete[] items;
    delete[] groups;
}


//only helps to initialize the DS
void UnionFind::makeSet(int i)
{
    group_uf * g = new group_uf();
    g->groupID = i;
    g->size = 1;
    tree_uf * t = new tree_uf();
    node_uf * n = new node_uf();
    n->father = nullptr;
    n->key = i;
    t->group = g;
    t->root = n;
    g->tree = t;
    groups[i] = g;
    items[i] = n;
    return;
}


//returns the groupID of the unioned group - it will be the groupID of the bigger group
// if both group have equal size so return groupID of group1
int UnionFind::union_groups(int group1, int group2)
{   
    
    if(groups[group1]->size > groups[group2]->size || (groups[group1]->size == groups[group2]->size && groups[group1]->groupID > groups[group2]->groupID))
    {
        groups[group2]->tree->root->father = groups[group1]->tree->root;
        delete groups[group2]->tree;
        //delete groups[group2];
        groups[group1]->size += groups[group2]->size;
        groups[group2]->tree = groups[group1]->tree;
        groups[group2]->groupID = groups[group1]->groupID;
        groups[group2]->size = groups[group1]->size;
        //groups[group2] = groups[group1];
        return group1;
    }
    groups[group1]->tree->root->father = groups[group2]->tree->root;
    delete groups[group1]->tree;
    //delete groups[group1];
    groups[group2]->size += groups[group1]->size;
    //groups[group1] = groups[group2];
    groups[group1]->tree = groups[group2]->tree;
    groups[group1]->groupID = groups[group2]->groupID;
    groups[group1]->size = groups[group2]->size;
    return group2;
}

//returns groupID and do kivutz maslulim
int UnionFind::find(int i)
{
    node_uf * n = items[i];
    while(n->father)
        n=n->father;
    node_uf * it = items[i];
    while(it->father)
    {
        node_uf * update = it;
        it = it->father;
        update->father = n;
    }
    return n->key;
}

