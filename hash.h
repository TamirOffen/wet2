#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <iostream>


// typedef enum {
//     ALLOCATION_ERROR = 0,
//     INVALID_INPUT,
//     SUCCESS,
//     FAILURE,
//     ALREADY_EXIST,
//     NOT_FOUND
// } StatusType;

template<class T>
struct node_hash
{   
    int key;
    T value;
    node_hash<T> * next;

    node_hash(int k, T v) : key(k), value(v), next(nullptr) {};
    node_hash() : key(-1), next(nullptr) {};
};

template<class T>
class HashTable
{   
    public:
    HashTable(int size);
    ~HashTable();
    
    void remove(int key);
    T find(int key);
    void insert(int key, T value);
    bool isInTable(int key);

    private:
    int size;
    node_hash<T> ** table;

};


//constructor
template<class T>
HashTable<T>::HashTable(int size) : size(size)
{
    table = new node_hash<T>*[size];
    for(int i=0; i<size; i++)
    {
        table[i] = new node_hash<T>();
        table[i]->key = -1; 
    }
}


//Destructor
template<class T>
HashTable<T>::~HashTable()
{
    for(int i=0; i<size;i++)
        {
            while(table[i])
            {   
                node_hash<T> * t = table[i];
                table[i] = table[i]->next;
                delete t;
            }
        }
    // free(table);
    delete[] table;
}


template<class T>
void HashTable<T>::insert(int key, T value)
{
    int new_key = key%size;
    node_hash<T> * it = table[new_key];
    if(it->key == -1) // check if table[new_key] is empty
    {
        table[new_key]->key = key;
        table[new_key]->value = value;
        table[new_key]->next = nullptr;
        return;
    }
    node_hash<T> * n = new node_hash<T>(key, value);
    while(it->next) // search for key in table[new_key]
    {
        if(it->key == key)\
        {   
            delete n;
            return;
        }
        it = it->next;
    }
    if(it->key == key)
    {   
        delete n;
        return;
    }
    it->next = n;
    return;
}



template<class T>
void HashTable<T>::remove(int key)
{
    int new_key = key%size;
    node_hash<T> * it = table[new_key];
    node_hash<T> * to_delete = nullptr;
    if(it->key == key) // if key is first in table[new_key]
    {   
        if(!it->next) // if key is the only node in table[new_key]
        {
            table[new_key]->key = -1;
            return;
        }
        table[new_key] = table[new_key]->next;
        //to_delete = it;
        delete it;
        return;
    }
    while(it->next) // search for key in table[new_key]
    {
        if(it->next->key == key)
        {
            to_delete = it->next;
            it->next = it->next->next;
            break;
        }
        it = it->next;
    }
    if(!to_delete)
        return;
    delete to_delete;
    return;
}


// return true if found key and false otherwise
template<class T>
T HashTable<T>::find(int key)
{
    int new_key = key%size;
    node_hash<T> * it = table[new_key];
    while(it)
    {
        if(it->key == key) {
            return it->value;
        }
        it = it->next;
    }
    T dummy_var;
    return dummy_var;
}

template<class T>
bool HashTable<T>::isInTable(int key) {
    int new_key = key%size;
    node_hash<T> * it = table[new_key];
    while(it)
    {
        if(it->key == key) {
            return true;
        }
        it = it->next;
    }
    return false;
}

#endif

