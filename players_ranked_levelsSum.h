
#ifndef PLAYERS_RANKED_LEVELSSUM_H
#define PLAYERS_RANKED_LEVELSSUM_H

#include <iostream>
using namespace std;

#include <cmath>

#include "Player.h"


struct PlayerInfo {
    int nl;  // sum of the levels of all of the nodes(players) in this subtree
    int n;  // number of nodes(players) in this subtree

    PlayerInfo(): nl(0), n(1) {};
    PlayerInfo(int nl, int n): nl(nl), n(n) {};

    // for testing:
    friend ostream& operator<<(ostream& os, PlayerInfo const & info) {
        return os << "(" << info.nl << "," << info.n << ")";
    }
};

struct node_l {
    Player player;
    PlayerInfo info;
    int height;

    node_l *left_son, *right_son, *parent; 
    node_l(): player(), info(), height(0), left_son(nullptr), right_son(nullptr), parent(nullptr) {};
    node_l(const Player newPlayer, const PlayerInfo newPlayerInfo) : player(newPlayer), info(newPlayerInfo), height(0), left_son(nullptr),
        right_son(nullptr), parent(nullptr) {}

};



class Players_Ranked_LevelsSum{
    void destroyTree(node_l* treeNode);
    void copyTree(node_l* src, node_l* dst);
    static node_l* bstSearch(node_l* treeNode, Player key);
    static node_l* bstAddNode(Players_Ranked_LevelsSum* tree, Player key, PlayerInfo info); 
    static node_l* bstRemoveNode(Players_Ranked_LevelsSum* tree, Player key); 
    void populateAlmostPerfectPlayers_Ranked_LevelsSum(node_l* treeRoot, node_l** mergedAB, int length, int* index);
    static void updateHeight(node_l* treeNode);
    int getUpdatedHeightOfNode(node_l* treeNode);
    static int bf(node_l* treeNode);
    void fillArray(node_l* treeRoot, node_l** array, int* index);
    void rotateRight(node_l* treeNode);
    void buildPerfectBinaryTree(node_l* treeRoot, int perfectTreeSize);
    void rotateLeft(node_l* treeNode);
    int getNumberOfNodes(node_l* treeRoot);
    void printTree(node_l* treeNode);
    void buildAlmostPerfectBinaryTree(node_l** treeRoot, int numOfNodes);
    int balance(node_l* treeNode);
    static node_l* getNextNodeInOrder(node_l* treeNode);
    void merge(node_l** dst, node_l** arr1, int length_arr1, node_l** arr2, int length_arr2);
    static void swapNodes(node_l* treeNode1, node_l* treeNode2);
    node_l* removeLastLeafs(node_l* treeRoot, int* numOfLeafsToDelete);
    node_l* getBiggestNode(node_l* root);
    void printReverseInOrder(node_l* treeNode);

    // added funcs:
    void updateInfo_Add(node_l* treeNode, Player player);
    void updateInfo_Remove(node_l* treeNode, Player player);
    void updateInfo_node(node_l* treeNode);
    void updateInfo_all(node_l* treeNode);
    Player select(int k, node_l* treeNode);
    void sumTo(Player player, node_l* treeNode, int* sum);

public:
    node_l* root; //root of the tree.
    node_l* biggest_node; //keeps track of the biggest_node in the tree.
    
    Players_Ranked_LevelsSum(); 
    ~Players_Ranked_LevelsSum(); 
    Players_Ranked_LevelsSum(const Players_Ranked_LevelsSum& tree); 

    Players_Ranked_LevelsSum& operator=(const Players_Ranked_LevelsSum& tree); //assignment operator

    node_l* search(const Player key) const;
    int addNode(const Player key, const PlayerInfo info); 
    int removeNode(Player key); 

    void printTree() { printTree(root); }  
    void treeClear() { destroyTree(root); root = nullptr; }
    void printReverseInOrder() { printReverseInOrder(root); } 
    int getNumberOfNodes() { return getNumberOfNodes(root); }

    void mergeTree(const Players_Ranked_LevelsSum& otherTree); //merges otherTree with this tree.

    // added funcs:
    Player select(int k); // returns the player with index k
    int sumTo(Player player); // returns the sum of levels of the players smaller than player 
    int getTotalLevels() {
        if(root == nullptr) return 0;
        return root->info.nl;
    }

    //tree print: TODO: DELETE BEFORE SUBMISSION
    void printBT(const string& prefix, const node_l* node, bool isLeft)
    {
        if( node != nullptr )
        {
            cout << prefix;
            cout << (isLeft ? "├──" : "└──" );

            // print the value of the node
            cout << node->player << node->info << endl;

            // enter the next tree level - left and right branch
            printBT( prefix + (isLeft ? "│      " : "        "), node->left_son, true);
            printBT( prefix + (isLeft ? "│      " : "        "), node->right_son, false);
        }
    }

    void printBT()
    {
        printBT("", root, false);    
    }

};

// public funcs:

void Players_Ranked_LevelsSum::printTree(node_l * treeNode) {
    if(treeNode == nullptr) return;
    printTree(treeNode->left_son);
    cout << treeNode->player << " BF: " << bf(treeNode) << " Height: " << treeNode->height << endl;//<<"|"<<height(treeNode)<< endl;
    printTree(treeNode->right_son);
}

 
Players_Ranked_LevelsSum ::Players_Ranked_LevelsSum() : root(nullptr) , biggest_node(nullptr) {

}

 
Players_Ranked_LevelsSum ::~Players_Ranked_LevelsSum() {
    destroyTree(root);
}

 
Players_Ranked_LevelsSum ::Players_Ranked_LevelsSum(const Players_Ranked_LevelsSum& tree) {
    if (tree.root != nullptr)
    {
        root = new node_l (tree.root->player, tree.root->info);
        root->height = tree.root->height;
        copyTree(tree.root, root);
    }
    else
    {
        root = nullptr;
    }
    biggest_node = getBiggestNode(root);
}

 
Players_Ranked_LevelsSum & Players_Ranked_LevelsSum ::operator=(const Players_Ranked_LevelsSum & tree) {
    if (this == &tree)
    {
        return *this;
    }

    node_l * temp = root;
    if (tree.root != nullptr)
    {
        node_l * temp = new node_l (tree.root->player, tree.root->info);
        copyTree(tree.root, temp);
    }
    else
    {
        node_l * temp = nullptr;
    }

    destroyTree(root);
    root = temp;
    biggest_node = getBiggestNode(root);
    return *this;
}

 
node_l * Players_Ranked_LevelsSum ::search(const Player key) const {
    node_l * found = bstSearch(root, key);
    if (found == nullptr || !(found->player == key))
    {
        return nullptr;
    }
    return found;
}

 
int Players_Ranked_LevelsSum ::addNode(const Player key, const PlayerInfo info) {
    node_l * new_node = bstAddNode(this, key, info);
    

    if (new_node == nullptr) //if a node with the key is already in the tree
    {
        return -1;
    }

    // updates n and ns in info of the rest of the tree (just the path)
    // updateInfo_Add(root, key); 
    
    while (new_node != root)  //search the node where the balance was broken
    {
        node_l * new_node_father = new_node->parent;
        if (new_node_father->height >= (new_node->height + 1))
        {
            break;
        }

        new_node_father->height = new_node->height + 1;

        if (balance(new_node_father))
        {
            break;
        }

        new_node = new_node_father;
    }

    updateInfo_all(root);
    

    biggest_node = getBiggestNode(root);

    return 1;
}

 
int Players_Ranked_LevelsSum ::removeNode(Player key) {
    if (root == nullptr)
    {
        return -1;
    }
    if(bstSearch(root, key) == nullptr) {
        return -1; // player not found
    }

    updateInfo_Remove(root, key);

    node_l * father_of_deleted = bstRemoveNode(this, key);
    if (father_of_deleted == nullptr && root != nullptr) //the key was not found
    {
        return -1;
    }

    // cout << father_of_deleted->player << endl;
    updateInfo_node(father_of_deleted);

    // balancing: rotations     
    bool is_current_left_of_next;
    while (father_of_deleted != nullptr)
    {
        node_l * next_node_in_path = father_of_deleted->parent;
        if (next_node_in_path != nullptr)
        {
            is_current_left_of_next = (next_node_in_path->left_son == father_of_deleted);
        }

        int initial_height = father_of_deleted->height;
        updateHeight(father_of_deleted);
        balance(father_of_deleted);
        if (next_node_in_path != nullptr)
        {
            if (is_current_left_of_next)
            {
                if (next_node_in_path->left_son->height == initial_height)
                {
                    break;
                }
            }
            else if (next_node_in_path->right_son->height == initial_height)
            {
                break;
            }
        }

        father_of_deleted = next_node_in_path;
    }
    
    updateInfo_all(root); //TODO: messes up runtime, but fixes bugs REMOVE COMMENT!!!!
    biggest_node = getBiggestNode(root);

    return 1;
}

 
void Players_Ranked_LevelsSum ::mergeTree(const Players_Ranked_LevelsSum& otherTree) {
    // get the number of nodes in each tree:
    int this_tree_size = getNumberOfNodes(root);
    int other_tree_size = getNumberOfNodes(otherTree.root);

    // sorted array of the nodes in both trees:
    int index_A = 0, index_B = 0;
    node_l ** A = new node_l *[this_tree_size];
    node_l ** B = new node_l *[other_tree_size];
    fillArray(root, A, &index_A);
    fillArray(otherTree.root, B, &index_B);

    // merge the arrays into 1 sorted array:
    node_l ** mergedAB = new node_l *[this_tree_size + other_tree_size];
    merge(mergedAB, A, this_tree_size, B, other_tree_size);

    // build an empty Players_Ranked_LevelsSum tree with 2n nodes:
    Players_Ranked_LevelsSum  mergedPlayers_Ranked_LevelsSum;
    buildAlmostPerfectBinaryTree(&(mergedPlayers_Ranked_LevelsSum.root), this_tree_size + other_tree_size);

    // populate mergedPlayers_Ranked_LevelsSum with values from mergedAB, in in-order traversal:
    int index_mergedAB = 0;
    populateAlmostPerfectPlayers_Ranked_LevelsSum(mergedPlayers_Ranked_LevelsSum.root, mergedAB, this_tree_size + other_tree_size, &index_mergedAB);
    // mergedPlayers_Ranked_LevelsSum.printTree();

    // free and copy mergedPlayers_Ranked_LevelsSum to this tree:
    treeClear();
    if(mergedPlayers_Ranked_LevelsSum.root == nullptr) {
        root = nullptr;
    } else {
        root = new node_l (mergedPlayers_Ranked_LevelsSum.root->player, mergedPlayers_Ranked_LevelsSum.root->info);
        root->height = mergedPlayers_Ranked_LevelsSum.root->height;
        copyTree(mergedPlayers_Ranked_LevelsSum.root, root);
    }

    biggest_node = getBiggestNode(root);

    updateInfo_all(root); 

    // free the memory used:
    for(int i = 0; i < this_tree_size; i++) {
        delete A[i];
    }
    delete[] A;

    for(int i = 0; i < other_tree_size; i++) {
        delete B[i];
    }
    delete[] B;

    for(int i = 0; i < this_tree_size + other_tree_size; i++) {
        delete mergedAB[i];
    }
    delete[] mergedAB;

}


Player Players_Ranked_LevelsSum::select(int k) {
    if(k <= 0) {
        return Player(-1,-1,-1); // illegal arguments
    }
    if(k>getNumberOfNodes()) {
        return select(getNumberOfNodes(), root);
    }
    return select(k, root);
}


int Players_Ranked_LevelsSum::sumTo(Player player) {
    //check for null player:
    Player pNull;
    if(player == pNull) return 0;
    int sum = 0;
    sumTo(player, root, &sum);
    return sum;
}


/***** private funcs *****/

Player Players_Ranked_LevelsSum::select(int k, node_l* treeNode) {
    int index_of_left_child = 0;
    if(treeNode != nullptr && treeNode->left_son != nullptr) {
        index_of_left_child = treeNode->left_son->info.n;
    }

    if(index_of_left_child == k - 1) {
        return treeNode->player;
    }
    else if(index_of_left_child > k - 1) {
        return select(k, treeNode->left_son);
    } else { //index_of_left_child < k - 1
        return select(k-index_of_left_child-1, treeNode->right_son);
    }
}
 

void Players_Ranked_LevelsSum::sumTo(Player player, node_l* treeNode, int* sum) {
    int nl_of_left_child = 0;
    if(treeNode != nullptr && treeNode->left_son != nullptr) {
        nl_of_left_child = treeNode->left_son->info.nl;
    }
    // cout << "ns_of_left_child: " << ns_of_left_child << endl;

    if(treeNode->player == player) {
        *sum += treeNode->player.getLevel() + nl_of_left_child;
        return;
    }

    if(treeNode->player < player) {
        *sum += treeNode->player.getLevel() + nl_of_left_child;
        sumTo(player, treeNode->right_son, sum);
    } else { // ... > ...
        sumTo(player, treeNode->left_son, sum);
    }
}



void Players_Ranked_LevelsSum ::destroyTree(node_l * treeNode) {
    if (treeNode == nullptr)
    {
        return;
    }

    destroyTree(treeNode->left_son);
    destroyTree(treeNode->right_son);
    delete treeNode;
}

 
void Players_Ranked_LevelsSum ::copyTree(node_l * src, node_l * dst) {
    if (src->left_son != nullptr)
    {
        dst->left_son = new node_l (src->left_son->player, src->left_son->info);
        dst->left_son->height = src->left_son->height;
        dst->left_son->parent = dst;
        copyTree(src->left_son, dst->left_son);
    }
    if (src->right_son != nullptr)
    {
        dst->right_son = new node_l (src->right_son->player, src->right_son->info);
        dst->right_son->height = src->right_son->height;
        dst->right_son->parent = dst;
        copyTree(src->right_son, dst->right_son);
    }
}


node_l* Players_Ranked_LevelsSum ::bstSearch(node_l * treeNode, Player key) {
    node_l* new_node = treeNode;
    if (new_node == nullptr) return nullptr;

    if (new_node->player == key) return new_node;
    if (key < new_node->player)
    {
        if (new_node->left_son == nullptr) return new_node;
        return bstSearch(new_node->left_son, key);
    }
    
    if (new_node->right_son == nullptr) return new_node;
    return bstSearch(new_node->right_son, key);
}

 
node_l* Players_Ranked_LevelsSum ::bstAddNode(Players_Ranked_LevelsSum * tree, Player key, PlayerInfo info) {
    node_l* father_of_new_node = bstSearch(tree->root, key);
    if (father_of_new_node == nullptr)
    {
        tree->root = new node_l (key, info);
        return tree->root;
    }
    if (father_of_new_node->player == key) return nullptr;
    node_l* new_node = new node_l (key, info);
    new_node->parent = father_of_new_node;
    if (key < father_of_new_node->player) father_of_new_node->left_son = new_node;
    else father_of_new_node->right_son = new_node;

    return new_node;
}

void Players_Ranked_LevelsSum::updateInfo_Add(node_l* treeNode, Player player) {
    if(treeNode->player == player) {
        treeNode->info.nl = player.getLevel();
        return;
    }

    treeNode->info.n += 1;
    treeNode->info.nl += player.getLevel();
    
    if(player < treeNode->player) {
        return updateInfo_Add(treeNode->left_son, player);
    }

    else {
        return updateInfo_Add(treeNode->right_son, player);
    }
}
 
void Players_Ranked_LevelsSum::updateInfo_Remove(node_l* treeNode, Player player) {
    if(treeNode->player == player) {
        return;
    }

    treeNode->info.n -= 1;
    
    if(player < treeNode->player) {
        return updateInfo_Remove(treeNode->left_son, player);
    }

    else {
        return updateInfo_Remove(treeNode->right_son, player);
    }
}

void Players_Ranked_LevelsSum::updateInfo_node(node_l* treeNode) {
    if(treeNode == nullptr) return;

    int toAdd_n = 0, toAdd_ns = 0;
    if(treeNode->left_son != nullptr) {
        toAdd_n += treeNode->left_son->info.n;
        toAdd_ns += treeNode->left_son->info.nl;

    }
    if(treeNode->right_son != nullptr) {
        toAdd_n += treeNode->right_son->info.n;
        toAdd_ns += treeNode->right_son->info.nl;
    }
    
    treeNode->info.n = 1 + toAdd_n;
    treeNode->info.nl = treeNode->player.getLevel() + toAdd_ns;
}

void Players_Ranked_LevelsSum::updateInfo_all(node_l* treeNode) {
    if(treeNode == nullptr) return;

    updateInfo_all(treeNode->left_son);
    updateInfo_all(treeNode->right_son);
    updateInfo_node(treeNode);
}


node_l * Players_Ranked_LevelsSum ::bstRemoveNode(Players_Ranked_LevelsSum * tree, Player key) {
    node_l * node_to_remove = bstSearch(tree->root, key);
    if (!(node_to_remove->player == key)) return nullptr;

    while (true)
    {
        node_l * node_to_remove_father = node_to_remove->parent;
        if (node_to_remove->left_son == nullptr && node_to_remove->right_son == nullptr) //the node is a leaf
        {
            if (tree->root == node_to_remove) //the leaf is the root of the tree
            {
                tree->root = nullptr;
                delete node_to_remove;
                return nullptr;
            }

            if (node_to_remove_father->left_son == node_to_remove) node_to_remove_father->left_son = nullptr;
            else node_to_remove_father->right_son = nullptr;
            delete node_to_remove;
            return node_to_remove_father;
        }

        else if (node_to_remove->left_son != nullptr && node_to_remove->right_son != nullptr)
        {
            node_l * next_node_in_order = getNextNodeInOrder(node_to_remove);
            swapNodes(next_node_in_order, node_to_remove);
            if (node_to_remove == tree->root) tree->root = next_node_in_order;
            continue;
        }

        else if (node_to_remove->left_son != nullptr)
        {
            if (node_to_remove_father == nullptr)
            {
                tree->root = node_to_remove->left_son;
                tree->root->parent = nullptr;
                delete node_to_remove;
                return tree->root;
            }
            else
            {
                if (node_to_remove_father->left_son == node_to_remove)
                {
                    node_to_remove_father->left_son = node_to_remove->left_son;
                    node_to_remove_father->left_son->parent = node_to_remove_father;
                }
                else
                {
                    node_to_remove_father->right_son = node_to_remove->left_son;
                    node_to_remove_father->right_son->parent = node_to_remove_father;
                }
                delete node_to_remove;
                return node_to_remove_father;
            }
        }
        else
        {
            if (node_to_remove_father == nullptr)
            {
                tree->root = node_to_remove->right_son;
                tree->root->parent = nullptr;
                delete node_to_remove;
                return tree->root;
            }
            else
            {
                if (node_to_remove_father->left_son == node_to_remove)
                {
                    node_to_remove_father->left_son = node_to_remove->right_son;
                    node_to_remove_father->left_son->parent = node_to_remove_father;
                }
                else
                {
                    node_to_remove_father->right_son = node_to_remove->right_son;
                    node_to_remove_father->right_son->parent = node_to_remove_father;
                }
                delete node_to_remove;
                return node_to_remove_father;
            }
        }
    }
}


void Players_Ranked_LevelsSum ::updateHeight(node_l * treeNode) {
    if (treeNode->left_son == nullptr && treeNode->right_son == nullptr)
    {
        treeNode->height = 0;
        return;
    }
    if (treeNode->left_son == nullptr)
    {
        treeNode->height = 1 + treeNode->right_son->height;
        return;
    }
    if (treeNode->right_son == nullptr)
    {
        treeNode->height = 1 + treeNode->left_son->height;
        return;
    }
    if (treeNode->left_son->height < treeNode->right_son->height)
    {
        treeNode->height = 1 + treeNode->right_son->height;
        return;
    }
    treeNode->height = 1 + treeNode->left_son->height;
}


int Players_Ranked_LevelsSum ::bf(node_l * treeNode) {   
    // balance factor = left_son - right_son (height)
    if (treeNode->left_son == nullptr && treeNode->right_son == nullptr)
        return 0;
    if (treeNode->left_son == nullptr)
        return -(treeNode->right_son->height + 1);
    if (treeNode->right_son == nullptr)
        return treeNode->left_son->height + 1;
    return treeNode->left_son->height - treeNode->right_son->height;
}


void Players_Ranked_LevelsSum ::rotateRight(node_l * treeNode) {
    node_l * new_left_son = treeNode->left_son;
    new_left_son->parent = treeNode->parent;
    if (treeNode->parent != nullptr)
    {
        if (treeNode->parent->left_son == treeNode)
        {
            treeNode->parent->left_son = new_left_son;
        }
        else
        {
            treeNode->parent->right_son = new_left_son;
        }
    }
    else
    {
        root = new_left_son;
    }
    treeNode->left_son = new_left_son->right_son;
    if (treeNode->left_son != nullptr)
    {
        treeNode->left_son->parent = treeNode;
    }
    new_left_son->right_son = treeNode;
    treeNode->parent = new_left_son;
    updateHeight(treeNode);
    updateHeight(new_left_son);
}


void Players_Ranked_LevelsSum ::rotateLeft(node_l * treeNode) {
    node_l * new_right_son = treeNode->right_son;
    new_right_son->parent = treeNode->parent;
    if (treeNode->parent != nullptr)
    {
        if (treeNode->parent->right_son == treeNode)
        {
            treeNode->parent->right_son = new_right_son;
        }
        else
        {
            treeNode->parent->left_son = new_right_son;
        }
    }
    else
    {
        root = new_right_son;
    }

    treeNode->right_son = new_right_son->left_son;
    if (treeNode->right_son != nullptr)
    {
        treeNode->right_son->parent = treeNode;
    }
    new_right_son->left_son = treeNode;
    treeNode->parent = new_right_son;

    updateHeight(treeNode);
    updateHeight(new_right_son);
}


node_l * Players_Ranked_LevelsSum ::getNextNodeInOrder(node_l * treeNode) {   
    // go right and all the way left to find the next in order
    node_l * next_in_order = treeNode->right_son;
    while (next_in_order->left_son != nullptr)
    {
        next_in_order = next_in_order->left_son;
    }
    return next_in_order;
}


void Players_Ranked_LevelsSum ::swapNodes(node_l * treeNode1, node_l * treeNode2) {   
    int node1_height = treeNode1->height;
    node_l * node1_right = treeNode1->right_son;
    node_l * node1_father = treeNode1->parent;
    node_l * node1_left = treeNode1->left_son;

    treeNode1->parent = treeNode2->parent;
    treeNode1->right_son = treeNode2->right_son;
    treeNode1->height = treeNode2->height;
    treeNode1->left_son = treeNode2->left_son;

    treeNode2->left_son = node1_left;
    treeNode2->height = node1_height;
    treeNode2->parent = node1_father;
    treeNode2->right_son = node1_right;

    if (treeNode2->parent == treeNode2) //if node1 is the left_son of treeNode2 this happens
    {
        treeNode1->right_son = treeNode2;
        treeNode2->parent = treeNode1;
    }

    if (treeNode1->left_son != nullptr)
    {
        treeNode1->left_son->parent = treeNode1;
    }
    if (treeNode1->right_son != nullptr)
    {
        treeNode1->right_son->parent = treeNode1;
    }
    if (treeNode2->left_son != nullptr)
    {
        treeNode2->left_son->parent = treeNode2;
    }
    if (treeNode2->right_son != nullptr)
    {
        treeNode2->right_son->parent = treeNode2;
    }

    if (treeNode1->parent != nullptr)
    {
        if (treeNode1->parent->left_son == treeNode2)
        {
            treeNode1->parent->left_son = treeNode1;
        }
        else
        {
            treeNode1->parent->right_son = treeNode1;
        }
    }
    if (treeNode2->parent != nullptr)
    {
        if (treeNode2->parent->left_son == treeNode1)
        {
            treeNode2->parent->left_son = treeNode2;
        }
        else
        {
            treeNode2->parent->right_son = treeNode2;
        }
    }

}


int Players_Ranked_LevelsSum ::balance(node_l * treeNode) {   
    // if not balanced - rotate. balanced == |bf| <= 1
    int balance_factor = bf(treeNode);
    if (balance_factor == 2)
    {
        //int left_son_balance = bf(treeNode->left_son);
        if (bf(treeNode->left_son) >= 0)  //Rotate LL
        {
            // cout<<"LL"<<endl;
            int n_A_l = 0, n_A_r = 0, n_A = 0, n_B = 0, n_B_r = 0;
            if(treeNode!=nullptr) n_B = treeNode->info.n;
            if(treeNode->left_son!=nullptr) n_A = treeNode->left_son->info.n;
            if(treeNode->right_son!=nullptr) n_B_r = treeNode->right_son->info.n;
            if(treeNode->left_son->left_son!=nullptr) n_A_l = treeNode->left_son->left_son->info.n;
            if(treeNode->left_son->right_son!=nullptr) n_A_r = treeNode->left_son->right_son->info.n;

            treeNode->left_son->info.n = 2 + n_A_l + n_A_r + n_B_r;
            treeNode->info.n = 1 + n_B_r + n_A_r;

            rotateRight(treeNode);
        }
        else if (bf(treeNode->left_son) == -1) //Rotate LR
        {
            // cout<<"LR"<<endl;
            int n_c_r = 0, n_b_l = 0, n_b_r = 0, n_a_l = 0;
            if(treeNode->right_son != nullptr) n_c_r = treeNode->right_son->info.n;
            if(treeNode->left_son->left_son != nullptr) n_a_l = treeNode->left_son->left_son->info.n;
            if(treeNode->left_son->right_son != nullptr) {
                if(treeNode->left_son->right_son->left_son != nullptr) n_b_l = treeNode->left_son->right_son->left_son->info.n;
                if(treeNode->left_son->right_son->right_son != nullptr) n_b_r = treeNode->left_son->right_son->right_son->info.n;
            }

            treeNode->info.n = 1 + n_b_r + n_c_r;
            treeNode->left_son->info.n = 1 + n_a_l + n_b_l;
            treeNode->left_son->right_son->info.n = 3 + n_c_r + n_b_l + n_b_r + n_a_l;

            rotateLeft(treeNode->left_son);
            rotateRight(treeNode);
        }
        return 1;
    }
    if (balance_factor == -2)
    {
        //int right_balance = bf(treeNode->right_son);
        if (bf(treeNode->right_son) <= 0) //Rotate RR
        {
            // cout<<"RR"<<endl;
            int n_B = 0, n_B_l = 0, n_A = 0, n_A_l = 0, n_A_r = 0;
            if(treeNode!=nullptr) n_B = treeNode->info.n;
            if(treeNode->right_son!=nullptr) n_A = treeNode->right_son->info.n;
            if(treeNode->left_son!=nullptr) n_B_l = treeNode->left_son->info.n;
            if(treeNode->right_son->left_son!=nullptr) n_A_l = treeNode->right_son->left_son->info.n;
            if(treeNode->right_son->right_son!=nullptr) n_A_r = treeNode->right_son->right_son->info.n;

            treeNode->info.n = 1 + n_B_l + n_A_l;
            treeNode->right_son->info.n = 2 + n_B_l + n_A_l + n_A_r;

            rotateLeft(treeNode);
        }
        else if (bf(treeNode->right_son) == 1) //Rotate RL
        {
            // cout<<"RL"<<endl;
            int n_c_l = 0, n_b_l = 0, n_b_r = 0, n_a_r = 0;
            if(treeNode->left_son != nullptr) n_c_l = treeNode->left_son->info.n;
            if(treeNode->right_son->right_son != nullptr) n_a_r = treeNode->right_son->right_son->info.n;
            if(treeNode->right_son->left_son != nullptr) {
                if(treeNode->right_son->left_son->left_son != nullptr) n_b_l = treeNode->right_son->left_son->left_son->info.n;
                if(treeNode->right_son->left_son->right_son != nullptr) n_b_r = treeNode->right_son->left_son->right_son->info.n;
            }

            treeNode->info.n = 1 + n_c_l + n_b_l;
            treeNode->right_son->info.n = 1 + n_b_r + n_a_r;
            treeNode->right_son->left_son->info.n = 3 + n_c_l + n_b_l + n_b_r + n_a_r;


            rotateRight(treeNode->right_son);
            rotateLeft(treeNode);
        }
        return 1;
    }
    return 0;

}


node_l * Players_Ranked_LevelsSum ::getBiggestNode(node_l * treeNode)
{   
    // all the way right
    if (treeNode == nullptr) return treeNode;
    node_l * current = treeNode;
    while (current->right_son != nullptr) current = current->right_son;
    return current;
}


void Players_Ranked_LevelsSum ::printReverseInOrder(node_l * treeNode) {
    if(treeNode == nullptr) return;
    
    printReverseInOrder(treeNode->right_son);
    cout << treeNode->player << " BF: " << bf(treeNode) << " Height: " << treeNode->height << endl;
    printReverseInOrder(treeNode->left_son);
    
}

 
int Players_Ranked_LevelsSum ::getUpdatedHeightOfNode(node_l * treeNode) {
    if(treeNode->left_son == nullptr && treeNode->right_son == nullptr) return 0;
    else if(treeNode->left_son == nullptr) return treeNode->right_son->height + 1;
    else if(treeNode->right_son == nullptr) return treeNode->left_son->height + 1;
    else {
        return max(treeNode->left_son->height, treeNode->right_son->height) + 1;
    }
}

 
int Players_Ranked_LevelsSum ::getNumberOfNodes(node_l * treeRoot) {
    //postorder traversal
    if(treeRoot == nullptr) {
        return 0;
    }
    return 1 + getNumberOfNodes(treeRoot->left_son) + getNumberOfNodes(treeRoot->right_son);
}



// mergeTree helper funcs:
void Players_Ranked_LevelsSum ::fillArray(node_l * treeRoot, node_l ** array, int* index) {
    if(treeRoot == nullptr) {
        return;
    }

    fillArray(treeRoot->left_son, array, index);
    array[*index] = new node_l (treeRoot->player, treeRoot->info); 
    (*index)++;
    fillArray(treeRoot->right_son, array, index);
}

 
void Players_Ranked_LevelsSum ::merge(node_l ** dst, node_l ** arr1, int length_arr1, node_l ** arr2, int length_arr2) {
    int dst_index = 0, arr1_index = 0, arr2_index = 0;
    while(arr1_index < length_arr1 && arr2_index < length_arr2) {
        if(arr1[arr1_index]->player < arr2[arr2_index]->player) {
            dst[dst_index] = new node_l (arr1[arr1_index]->player, arr1[arr1_index]->info);
            arr1_index++;
        } else {
            dst[dst_index] = new node_l (arr2[arr2_index]->player, arr2[arr2_index]->info);
            arr2_index++;
        }
        dst_index++;
    }

    while(arr1_index < length_arr1) {
        dst[dst_index] = new node_l (arr1[arr1_index]->player, arr1[arr1_index]->info);
        arr1_index++;
        dst_index++;
    }

    while(arr2_index < length_arr2) {
        dst[dst_index] = new node_l (arr2[arr2_index]->player, arr2[arr2_index]->info);
        arr2_index++;
        dst_index++;
    }

}

 
void Players_Ranked_LevelsSum ::buildPerfectBinaryTree(node_l * treeRoot, int height) {
    if(height == 0) {
        return;
    }

    // treeRoot->left_son = createNewNode(0,0);
    treeRoot->left_son = new node_l ;
    treeRoot->left_son->height = height-1;
    treeRoot->left_son->parent = treeRoot;
    // treeRoot->right_son = createNewNode(0,0);
    treeRoot->right_son = new node_l ;
    treeRoot->right_son->height = height-1;
    treeRoot->right_son->parent = treeRoot;

    buildPerfectBinaryTree(treeRoot->left_son, height-1);
    buildPerfectBinaryTree(treeRoot->right_son, height-1);
}

 
node_l * Players_Ranked_LevelsSum ::removeLastLeafs(node_l * treeRoot, int* numOfLeafsToDelete) {
    // we are doing backwards in-order traversal
    if(treeRoot == nullptr) {
        return nullptr;
    }
    if(*numOfLeafsToDelete == 0) {
        return treeRoot;
    }
    treeRoot->right_son = removeLastLeafs(treeRoot->right_son, numOfLeafsToDelete);
    if(treeRoot->height == 0) {
        //node is a leaf
        if(treeRoot->parent != nullptr) {
            if(treeRoot->parent->right_son == treeRoot) {
                treeRoot->parent->right_son = nullptr;
            } else {
                treeRoot->parent->left_son = nullptr;
            }
            treeRoot->parent->height = getUpdatedHeightOfNode(treeRoot->parent);
        }
        delete treeRoot;
        treeRoot = nullptr; 
        *numOfLeafsToDelete -= 1;
        return nullptr;
    }
    treeRoot->left_son = removeLastLeafs(treeRoot->left_son, numOfLeafsToDelete);
    return treeRoot;

}

 
void Players_Ranked_LevelsSum ::buildAlmostPerfectBinaryTree(node_l ** treeRoot, int numOfNodes) {
    int perfectTreeHeight = ceil(log2(1+numOfNodes)) - 1;
    int numOfNodesInPerfectTree = pow(2, perfectTreeHeight + 1) - 1;

    *treeRoot = new node_l ;
    (*treeRoot)->height = perfectTreeHeight;
    buildPerfectBinaryTree(*treeRoot, perfectTreeHeight);

    int numOfLeafsToDelete = numOfNodesInPerfectTree - numOfNodes;
    *treeRoot = removeLastLeafs(*treeRoot, &numOfLeafsToDelete);

}

 
void Players_Ranked_LevelsSum ::populateAlmostPerfectPlayers_Ranked_LevelsSum(node_l * treeRoot, node_l ** mergedAB, int length, int* index) {
    // in-order traversal:
    if(*index == length) {
        return;
    }
    if(treeRoot == nullptr) {
        return;
    }
    populateAlmostPerfectPlayers_Ranked_LevelsSum(treeRoot->left_son, mergedAB, length, index);
    treeRoot->player = mergedAB[*index]->player;
    treeRoot->info = mergedAB[*index]->info;
    (*index)++;
    populateAlmostPerfectPlayers_Ranked_LevelsSum(treeRoot->right_son, mergedAB, length, index);
}



#endif //PLAYERS_RANKED_LEVELSSUM_H

