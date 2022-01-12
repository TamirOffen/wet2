
#ifndef PLAYERS_RANKED_SCORE_H
#define PLAYERS_RANKED_SCORE_H

#include <iostream>
using namespace std;

#include <cmath>

#include "Player.h"


struct PlayerScoreInfo {
    int ns;  // number of players in the subtree with score(of the player) = score(of the tree).
    int n;  // number of nodes(players) in this subtree.

    PlayerScoreInfo(): ns(0), n(1) {};
    PlayerScoreInfo(int ns, int n): ns(ns), n(n) {};

    // for testing:
    friend ostream& operator<<(ostream& os, PlayerScoreInfo const & info) {
        return os << "(" << info.ns << "," << info.n << ")";
    }
};

struct node_s {
    Player player;
    PlayerScoreInfo info;
    int height;

    node_s *left_son, *right_son, *parent; 
    node_s(): player(), info(), height(0), left_son(nullptr), right_son(nullptr), parent(nullptr) {};
    node_s(const Player newPlayer, const PlayerScoreInfo newPlayerScoreInfo) : player(newPlayer), info(newPlayerScoreInfo), height(0), left_son(nullptr),
        right_son(nullptr), parent(nullptr) {}

};



class Players_Ranked_Score {
    

    void destroyTree(node_s* treeNode);
    void copyTree(node_s* src, node_s* dst);
    static node_s* bstSearch(node_s* treeNode, Player key);
    static node_s* bstAddNode(Players_Ranked_Score* tree, Player key, PlayerScoreInfo info); 
    static node_s* bstRemoveNode(Players_Ranked_Score* tree, Player key); 
    void populateAlmostPerfectPlayers_Ranked_Score(node_s* treeRoot, node_s** mergedAB, int length, int* index);
    static void updateHeight(node_s* treeNode);
    int getUpdatedHeightOfNode(node_s* treeNode);
    static int bf(node_s* treeNode);
    void fillArray(node_s* treeRoot, node_s** array, int* index);
    void rotateRight(node_s* treeNode);
    void buildPerfectBinaryTree(node_s* treeRoot, int perfectTreeSize);
    void rotateLeft(node_s* treeNode);
    int getNumberOfNodes(node_s* treeRoot);
    void printTree(node_s* treeNode);
    void buildAlmostPerfectBinaryTree(node_s** treeRoot, int numOfNodes);
    int balance(node_s* treeNode);
    static node_s* getNextNodeInOrder(node_s* treeNode);
    void merge(node_s** dst, node_s** arr1, int length_arr1, node_s** arr2, int length_arr2);
    static void swapNodes(node_s* treeNode1, node_s* treeNode2);
    node_s* removeLastLeafs(node_s* treeRoot, int* numOfLeafsToDelete);
    node_s* getBiggestNode(node_s* root);
    void printReverseInOrder(node_s* treeNode);

    // added funcs:
    void updateInfo_Add(node_s* treeNode, Player player);
    void updateInfo_Remove(node_s* treeNode, Player player);
    void updateInfo_node(node_s* treeNode);
    void updateInfo_all(node_s* treeNode);
    Player select(int k, node_s* treeNode);
    void sumTo_score(Player player, node_s* treeNode, int* sum);
    void sumTo_players(Player player, node_s* treeNode, int* sum);
    int numOfPlayersLessThan(node_s* treeNode, int level, int score_equal, int* scoreReturn);

public:
    int score;
    node_s* root; //root of the tree.
    node_s* biggest_node; //keeps track of the biggest_node in the tree.
    
    int getTreeScore() { return score; }
    
    Players_Ranked_Score();
    Players_Ranked_Score(int score); 
    ~Players_Ranked_Score(); 
    Players_Ranked_Score(const Players_Ranked_Score& tree); 

    Players_Ranked_Score& operator=(const Players_Ranked_Score& tree); //assignment operator

    node_s* search(const Player key) const;
    int addNode(const Player key, const PlayerScoreInfo info); 
    int removeNode(Player key); 

    void printTree() { printTree(root); }  
    void treeClear() { destroyTree(root); root = nullptr; }
    void printReverseInOrder() { printReverseInOrder(root); } 
    int getNumberOfNodes() { return getNumberOfNodes(root); }

    void mergeTree(const Players_Ranked_Score& otherTree); //merges otherTree with this tree.

    // added funcs:
    Player select(int k); // returns the player with index k
    int sumTo_score(Player player); // returns the number of players with score=score less than player.
    int sumTo_players(Player player); // returns the number of players less than player.
    
    // returns number of players with a level less than or equal to "level".
    // scoreReturn = ^^^ but only the players with a score equal to score_equal.
    int numOfPlayersLessThan(int level, int score_equal, int* scoreReturn);


    //tree print: TODO: DELETE BEFORE SUBMISSION
    void printBT(const string& prefix, const node_s* node, bool isLeft)
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

void Players_Ranked_Score::printTree(node_s * treeNode) {
    if(treeNode == nullptr) return;
    printTree(treeNode->left_son);
    cout << treeNode->player << " BF: " << bf(treeNode) << " Height: " << treeNode->height << endl;//<<"|"<<height(treeNode)<< endl;
    printTree(treeNode->right_son);
}

 
Players_Ranked_Score ::Players_Ranked_Score(): score(0), root(nullptr) , biggest_node(nullptr) {
}

Players_Ranked_Score ::Players_Ranked_Score(int score): score(score), root(nullptr) , biggest_node(nullptr) {
}

 
Players_Ranked_Score ::~Players_Ranked_Score() {
    destroyTree(root);
}

 
Players_Ranked_Score ::Players_Ranked_Score(const Players_Ranked_Score& tree) {
    if (tree.root != nullptr)
    {
        root = new node_s (tree.root->player, tree.root->info);
        root->height = tree.root->height;
        score = tree.score;
        copyTree(tree.root, root);
    }
    else
    {
        root = nullptr;
        score = tree.score;
    }
    biggest_node = getBiggestNode(root);
}

 
Players_Ranked_Score & Players_Ranked_Score ::operator=(const Players_Ranked_Score & tree) {
    if (this == &tree)
    {
        return *this;
    }

    node_s * temp = root;
    if (tree.root != nullptr)
    {
        node_s * temp = new node_s (tree.root->player, tree.root->info);
        copyTree(tree.root, temp);
    }
    else
    {
        node_s * temp = nullptr;
    }

    destroyTree(root);
    root = temp;
    biggest_node = getBiggestNode(root);
    score = tree.score;
    return *this;
}

 
node_s * Players_Ranked_Score ::search(const Player key) const {
    node_s * found = bstSearch(root, key);
    if (found == nullptr || !(found->player == key))
    {
        return nullptr;
    }
    return found;
}

 
int Players_Ranked_Score ::addNode(const Player key, const PlayerScoreInfo info) {
    node_s * new_node = bstAddNode(this, key, info);
    
    if (new_node == nullptr) //if a node with the key is already in the tree
    {
        return -1;
    }

    // updates n and ns in info of the rest of the tree (just the path)
    // updateInfo_Add(root, key); 
    
    while (new_node != root)  //search the node where the balance was broken
    {
        node_s * new_node_father = new_node->parent;
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

 
int Players_Ranked_Score ::removeNode(Player key) {
    if (root == nullptr)
    {
        return -1;
    }
    if(bstSearch(root, key) == nullptr) {
        return -1; // player not found
    }

    updateInfo_Remove(root, key);

    node_s * father_of_deleted = bstRemoveNode(this, key);
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
        node_s * next_node_in_path = father_of_deleted->parent;
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

 
void Players_Ranked_Score ::mergeTree(const Players_Ranked_Score& otherTree) {
    // get the number of nodes in each tree:
    int this_tree_size = getNumberOfNodes(root);
    int other_tree_size = getNumberOfNodes(otherTree.root);

    // sorted array of the nodes in both trees:
    int index_A = 0, index_B = 0;
    node_s ** A = new node_s *[this_tree_size];
    node_s ** B = new node_s *[other_tree_size];
    fillArray(root, A, &index_A);
    fillArray(otherTree.root, B, &index_B);

    // merge the arrays into 1 sorted array:
    node_s ** mergedAB = new node_s *[this_tree_size + other_tree_size];
    merge(mergedAB, A, this_tree_size, B, other_tree_size);

    // build an empty Players_Ranked_Score tree with 2n nodes:
    Players_Ranked_Score  mergedPlayers_Ranked_Score(otherTree.score);
    buildAlmostPerfectBinaryTree(&(mergedPlayers_Ranked_Score.root), this_tree_size + other_tree_size);

    // populate mergedPlayers_Ranked_Score with values from mergedAB, in in-order traversal:
    int index_mergedAB = 0;
    populateAlmostPerfectPlayers_Ranked_Score(mergedPlayers_Ranked_Score.root, mergedAB, this_tree_size + other_tree_size, &index_mergedAB);
    // mergedPlayers_Ranked_Score.printTree();

    // free and copy mergedPlayers_Ranked_Score to this tree:
    treeClear();
    if(mergedPlayers_Ranked_Score.root == nullptr) {
        root = nullptr;
    } else {
        root = new node_s (mergedPlayers_Ranked_Score.root->player, mergedPlayers_Ranked_Score.root->info);
        root->height = mergedPlayers_Ranked_Score.root->height;
        copyTree(mergedPlayers_Ranked_Score.root, root);
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


Player Players_Ranked_Score::select(int k) {
    if(k <= 0 || k > getNumberOfNodes()) {
        return Player(-1,-1,-1); // illegal arguments
    }
    return select(k, root);
}


int Players_Ranked_Score::sumTo_score(Player player) {
    //check for null player:
    // Player pNull;
    // if(player == pNull) return 0;
    int sum = 0;
    sumTo_score(player, root, &sum);
    return sum;
}

int Players_Ranked_Score::sumTo_players(Player player) {
    //check for null player:
    // Player pNull;
    // if(player == pNull) return 0;
    int sum = 0;
    sumTo_players(player, root, &sum);
    return sum;
}


int Players_Ranked_Score::numOfPlayersLessThan(int level, int score_equal, int* scoreReturn) {
    Player pTest;
    pTest.changeLevel(level + 1);
    *scoreReturn = sumTo_score(pTest);
    return sumTo_players(pTest);
}


/***** private funcs *****/

Player Players_Ranked_Score::select(int k, node_s* treeNode) {
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
 

void Players_Ranked_Score::sumTo_score(Player player, node_s* treeNode, int* sum) {
    if(treeNode == nullptr) return;

    int ns_of_left_child = 0;
    if(treeNode != nullptr && treeNode->left_son != nullptr) {
        ns_of_left_child = treeNode->left_son->info.ns;
    }
    // cout << "ns_of_left_child: " << ns_of_left_child << endl;

    if(treeNode->player == player) {
        if(treeNode->player.getScore() == score) {
            (*sum)++;
        }
        *sum += ns_of_left_child;
        return;
    }

    if(treeNode->player < player) {
        if(treeNode->player.getScore() == score) {
            (*sum)++;
        }
        *sum += ns_of_left_child;
        sumTo_score(player, treeNode->right_son, sum);
    } else { // ... > ...
        sumTo_score(player, treeNode->left_son, sum);
    }
}


void Players_Ranked_Score::sumTo_players(Player player, node_s* treeNode, int* sum) {
    if(treeNode == nullptr) return;

    int n_of_left_child = 0;
    if(treeNode != nullptr && treeNode->left_son != nullptr) {
        n_of_left_child = treeNode->left_son->info.n;
    }

    if(treeNode->player == player) {
        *sum += 1 + n_of_left_child;
        return;
    }

    if(treeNode->player < player) {
        *sum += 1+ n_of_left_child;
        sumTo_players(player, treeNode->right_son, sum);
    } else { // ... > ...
        sumTo_players(player, treeNode->left_son, sum);
    }
}


void Players_Ranked_Score ::destroyTree(node_s * treeNode) {
    if (treeNode == nullptr)
    {
        return;
    }

    destroyTree(treeNode->left_son);
    destroyTree(treeNode->right_son);
    delete treeNode;
}

 
void Players_Ranked_Score ::copyTree(node_s * src, node_s * dst) {
    if (src->left_son != nullptr)
    {
        dst->left_son = new node_s (src->left_son->player, src->left_son->info);
        dst->left_son->height = src->left_son->height;
        dst->left_son->parent = dst;
        copyTree(src->left_son, dst->left_son);
    }
    if (src->right_son != nullptr)
    {
        dst->right_son = new node_s (src->right_son->player, src->right_son->info);
        dst->right_son->height = src->right_son->height;
        dst->right_son->parent = dst;
        copyTree(src->right_son, dst->right_son);
    }
}


node_s* Players_Ranked_Score ::bstSearch(node_s * treeNode, Player key) {
    node_s* new_node = treeNode;
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

 
node_s* Players_Ranked_Score ::bstAddNode(Players_Ranked_Score * tree, Player key, PlayerScoreInfo info) {
    node_s* father_of_new_node = bstSearch(tree->root, key);
    if (father_of_new_node == nullptr)
    {
        tree->root = new node_s (key, info);
        return tree->root;
    }
    if (father_of_new_node->player == key) return nullptr;
    node_s* new_node = new node_s (key, info);
    new_node->parent = father_of_new_node;
    if (key < father_of_new_node->player) father_of_new_node->left_son = new_node;
    else father_of_new_node->right_son = new_node;

    return new_node;
}

void Players_Ranked_Score::updateInfo_Add(node_s* treeNode, Player player) {
    if(treeNode->player == player) {
        return;
    }

    treeNode->info.n += 1;
    
    if(player < treeNode->player) {
        return updateInfo_Add(treeNode->left_son, player);
    }

    else {
        return updateInfo_Add(treeNode->right_son, player);
    }
}
 
void Players_Ranked_Score::updateInfo_Remove(node_s* treeNode, Player player) {
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

void Players_Ranked_Score::updateInfo_node(node_s* treeNode) {
    if(treeNode == nullptr) return;

    int toAdd_n = 0, toAdd_ns = 0;
    if(treeNode->left_son != nullptr) {
        toAdd_n += treeNode->left_son->info.n;
        toAdd_ns += treeNode->left_son->info.ns;

    }
    if(treeNode->right_son != nullptr) {
        toAdd_n += treeNode->right_son->info.n;
        toAdd_ns += treeNode->right_son->info.ns;
    }

    if(treeNode->player.getScore() == score) {
        toAdd_ns++;
    }
    
    treeNode->info.n = 1 + toAdd_n;
    treeNode->info.ns = toAdd_ns;
}

void Players_Ranked_Score::updateInfo_all(node_s* treeNode) {
    if(treeNode == nullptr) return;

    updateInfo_all(treeNode->left_son);
    updateInfo_all(treeNode->right_son);
    updateInfo_node(treeNode);
}


node_s * Players_Ranked_Score ::bstRemoveNode(Players_Ranked_Score * tree, Player key) {
    node_s * node_to_remove = bstSearch(tree->root, key);
    if (!(node_to_remove->player == key)) return nullptr;

    while (true)
    {
        node_s * node_to_remove_father = node_to_remove->parent;
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
            node_s * next_node_in_order = getNextNodeInOrder(node_to_remove);
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


void Players_Ranked_Score ::updateHeight(node_s * treeNode) {
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


int Players_Ranked_Score ::bf(node_s * treeNode) {   
    // balance factor = left_son - right_son (height)
    if (treeNode->left_son == nullptr && treeNode->right_son == nullptr)
        return 0;
    if (treeNode->left_son == nullptr)
        return -(treeNode->right_son->height + 1);
    if (treeNode->right_son == nullptr)
        return treeNode->left_son->height + 1;
    return treeNode->left_son->height - treeNode->right_son->height;
}


void Players_Ranked_Score ::rotateRight(node_s * treeNode) {
    node_s * new_left_son = treeNode->left_son;
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


void Players_Ranked_Score ::rotateLeft(node_s * treeNode) {
    node_s * new_right_son = treeNode->right_son;
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


node_s * Players_Ranked_Score ::getNextNodeInOrder(node_s * treeNode) {   
    // go right and all the way left to find the next in order
    node_s * next_in_order = treeNode->right_son;
    while (next_in_order->left_son != nullptr)
    {
        next_in_order = next_in_order->left_son;
    }
    return next_in_order;
}


void Players_Ranked_Score ::swapNodes(node_s * treeNode1, node_s * treeNode2) {   
    int node1_height = treeNode1->height;
    node_s * node1_right = treeNode1->right_son;
    node_s * node1_father = treeNode1->parent;
    node_s * node1_left = treeNode1->left_son;

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


int Players_Ranked_Score ::balance(node_s * treeNode) {   
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


node_s * Players_Ranked_Score ::getBiggestNode(node_s * treeNode)
{   
    // all the way right
    if (treeNode == nullptr) return treeNode;
    node_s * current = treeNode;
    while (current->right_son != nullptr) current = current->right_son;
    return current;
}


void Players_Ranked_Score ::printReverseInOrder(node_s * treeNode) {
    if(treeNode == nullptr) return;
    
    printReverseInOrder(treeNode->right_son);
    cout << treeNode->player << " BF: " << bf(treeNode) << " Height: " << treeNode->height << endl;
    printReverseInOrder(treeNode->left_son);
    
}

 
int Players_Ranked_Score ::getUpdatedHeightOfNode(node_s * treeNode) {
    if(treeNode->left_son == nullptr && treeNode->right_son == nullptr) return 0;
    else if(treeNode->left_son == nullptr) return treeNode->right_son->height + 1;
    else if(treeNode->right_son == nullptr) return treeNode->left_son->height + 1;
    else {
        return max(treeNode->left_son->height, treeNode->right_son->height) + 1;
    }
}

 
int Players_Ranked_Score ::getNumberOfNodes(node_s * treeRoot) {
    //postorder traversal
    if(treeRoot == nullptr) {
        return 0;
    }
    return 1 + getNumberOfNodes(treeRoot->left_son) + getNumberOfNodes(treeRoot->right_son);
}



// mergeTree helper funcs:
void Players_Ranked_Score ::fillArray(node_s * treeRoot, node_s ** array, int* index) {
    if(treeRoot == nullptr) {
        return;
    }

    fillArray(treeRoot->left_son, array, index);
    array[*index] = new node_s (treeRoot->player, treeRoot->info); 
    (*index)++;
    fillArray(treeRoot->right_son, array, index);
}

 
void Players_Ranked_Score ::merge(node_s ** dst, node_s ** arr1, int length_arr1, node_s ** arr2, int length_arr2) {
    int dst_index = 0, arr1_index = 0, arr2_index = 0;
    while(arr1_index < length_arr1 && arr2_index < length_arr2) {
        if(arr1[arr1_index]->player < arr2[arr2_index]->player) {
            dst[dst_index] = new node_s (arr1[arr1_index]->player, arr1[arr1_index]->info);
            arr1_index++;
        } else {
            dst[dst_index] = new node_s (arr2[arr2_index]->player, arr2[arr2_index]->info);
            arr2_index++;
        }
        dst_index++;
    }

    while(arr1_index < length_arr1) {
        dst[dst_index] = new node_s (arr1[arr1_index]->player, arr1[arr1_index]->info);
        arr1_index++;
        dst_index++;
    }

    while(arr2_index < length_arr2) {
        dst[dst_index] = new node_s (arr2[arr2_index]->player, arr2[arr2_index]->info);
        arr2_index++;
        dst_index++;
    }

}

 
void Players_Ranked_Score ::buildPerfectBinaryTree(node_s * treeRoot, int height) {
    if(height == 0) {
        return;
    }

    // treeRoot->left_son = createNewNode(0,0);
    treeRoot->left_son = new node_s ;
    treeRoot->left_son->height = height-1;
    treeRoot->left_son->parent = treeRoot;
    // treeRoot->right_son = createNewNode(0,0);
    treeRoot->right_son = new node_s ;
    treeRoot->right_son->height = height-1;
    treeRoot->right_son->parent = treeRoot;

    buildPerfectBinaryTree(treeRoot->left_son, height-1);
    buildPerfectBinaryTree(treeRoot->right_son, height-1);
}

 
node_s * Players_Ranked_Score ::removeLastLeafs(node_s * treeRoot, int* numOfLeafsToDelete) {
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

 
void Players_Ranked_Score ::buildAlmostPerfectBinaryTree(node_s ** treeRoot, int numOfNodes) {
    int perfectTreeHeight = ceil(log2(1+numOfNodes)) - 1;
    int numOfNodesInPerfectTree = pow(2, perfectTreeHeight + 1) - 1;

    *treeRoot = new node_s ;
    (*treeRoot)->height = perfectTreeHeight;
    buildPerfectBinaryTree(*treeRoot, perfectTreeHeight);

    int numOfLeafsToDelete = numOfNodesInPerfectTree - numOfNodes;
    *treeRoot = removeLastLeafs(*treeRoot, &numOfLeafsToDelete);

}

 
void Players_Ranked_Score ::populateAlmostPerfectPlayers_Ranked_Score(node_s * treeRoot, node_s ** mergedAB, int length, int* index) {
    // in-order traversal:
    if(*index == length) {
        return;
    }
    if(treeRoot == nullptr) {
        return;
    }
    populateAlmostPerfectPlayers_Ranked_Score(treeRoot->left_son, mergedAB, length, index);
    treeRoot->player = mergedAB[*index]->player;
    treeRoot->info = mergedAB[*index]->info;
    (*index)++;
    populateAlmostPerfectPlayers_Ranked_Score(treeRoot->right_son, mergedAB, length, index);
}



#endif //PLAYERS_RANKED_SCORE_H

