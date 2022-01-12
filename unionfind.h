#ifndef NEWUNIONFIND_H
#define NEWUNIONFIND_H

#include <stdio.h>
#include <iostream>
using namespace std;


class UnionFind
{
    public:
    //int ** groups;
    int * parents;
    int * sizes_arr;
    int size;
    int current_size;

    UnionFind(int s);
    ~UnionFind();
    int find(int i);
    void union_groups(int group1, int group2);
    void makeSet(int i);
    void print_func();
    int getCurrentSize();

};

#endif