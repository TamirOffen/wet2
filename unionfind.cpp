#include <stdio.h>
#include <iostream>
#include "unionfind.h"
using namespace std;

#include <cassert>


UnionFind::UnionFind(int s) : size(s+1), current_size(s)
{   
    //groups = new int*[s];
    parents = new int[s+1];
    sizes_arr = new int[s+1];
    for(int i=1; i<s+1; i++)
        makeSet(i);
}

UnionFind::~UnionFind()
{
    //delete[] groups;
    delete[] parents;
    delete[] sizes_arr;
}

void UnionFind::makeSet(int i)
{
    //groups[i] = new int;
    //*(groups[i]) = i;
    parents[i] = 0;
    sizes_arr[i] = 1;
    return;
}

int UnionFind::find(int i)
{
    int it = i;
    while(parents[it])
        it = parents[it];
    while(parents[i])
    {
        int update = i;
        i = parents[i];
        parents[update] = it;
    }
    return it;
}

void UnionFind::union_groups(int group1, int group2)
{   /*
    int n1 = *(groups[group1]);
    int n2 = *(groups[group2]);
    if(n1 == n2)
        return;
    if(sizes_arr[n1] > sizes_arr[n2] || (sizes_arr[n1] == sizes_arr[n2] && n1 > n2))
    {
        parents[n2] = n1;
        sizes_arr[n1] += sizes_arr[n2];
        groups[group2] = &(*(groups[group1]));
        sizes_arr[n2] = 0;
        return;
    }
    parents[n1] = n2;
    sizes_arr[n2] += sizes_arr[n1];
    groups[group1] = groups[group2];
    sizes_arr[n1] = 0;
    return;
    */
    //with find -> O(log*)

    int n1 = find(group1);
    int n2 = find(group2);
    if(n1 == n2)
        return;
    if(sizes_arr[n1] > sizes_arr[n2] || (sizes_arr[n1] == sizes_arr[n2] && n1 > n2))
    {
        parents[n2] = n1;
        sizes_arr[n1] += sizes_arr[n2];
        sizes_arr[n2] = 0;
        current_size--;
        return;
    }
    parents[n1] = n2;
    sizes_arr[n2] += sizes_arr[n1];
    sizes_arr[n1] = 0;
    current_size--;
    return;

}

int UnionFind::getCurrentSize()
{
    return current_size;
}
