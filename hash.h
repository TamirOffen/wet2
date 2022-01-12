#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <iostream>


typedef enum {
    ALLOCATION_ERROR = 0,
    INVALID_INPUT,
    SUCCESS,
    FAILURE,
    ALREADY_EXIST,
    NOT_FOUND
} StatusType;

template<class T>
struct node_hash
{   
    int key;
    T value;
    node_hash<T> * next = nullptr;

    node_hash(int k, T v) : key(k), value(v) {};
    node_hash() : key(-1) {};
};

template<class T>
class HashTable
{   
    public:
    HashTable(int size);
    ~HashTable();
    
    StatusType remove(int key);
    int find(int key);
    StatusType insert(int key, T value);

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
StatusType HashTable<T>::insert(int key, T value)
{
    int new_key = key%size;
    node_hash<T> * it = table[new_key];
    if(it->key == -1) // check if table[new_key] is empty
    {
        table[new_key]->key = key;
        table[new_key]->value = value;
        table[new_key]->next = nullptr;
        return SUCCESS;
    }
    node_hash<T> * n = new node_hash<T>(key, value);
    while(it->next) // search for key in table[new_key]
    {
        if(it->key == key)\
        {   
            delete n;
            return ALREADY_EXIST;
        }
        it = it->next;
    }
    if(it->key == key)
    {   
        delete n;
        return ALREADY_EXIST;
    }
    it->next = n;
    return SUCCESS;
}



template<class T>
StatusType HashTable<T>::remove(int key)
{
    int new_key = key%size;
    node_hash<T> * it = table[new_key];
    node_hash<T> * to_delete = nullptr;
    if(it->key == key) // if key is first in table[new_key]
    {   
        if(!it->next) // if key is the only node in table[new_key]
        {
            table[new_key]->key = -1;
            return SUCCESS;
        }
        table[new_key] = table[new_key]->next;
        //to_delete = it;
        delete it;
        return SUCCESS;
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
        return NOT_FOUND;
    delete to_delete;
    return SUCCESS;
}


// return 1 if found key and -1 otherwise
template<class T>
int HashTable<T>::find(int key)
{
    int new_key = key%size;
    node_hash<T> * it = table[new_key];
    while(it)
    {
        if(it->key == key)
            return 1;
        it = it->next;
    }
    return -1;
}

#endif

