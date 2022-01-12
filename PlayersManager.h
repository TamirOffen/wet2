
#ifndef PLAYERS_MANAGER_H
#define PLAYERS_MANAGER_H

#include <iostream>
using namespace std;

#include "Player.h"
#include "unionfind.h"
#include "players_ranked_levelsSum.h"
#include "players_ranked_score.h"


// an element inside of the groups array in PM.
struct group {
    int size;
    Players_Ranked_LevelsSum players_Ranked_LevelsSum;
    Players_Ranked_Score* scores; // array of size scale+1, of Players_Ranked_Score trees.

};


class PlayersManager {
    int scale; // for every player: 1 <= player.score <= scale <= 200.
    int k; // the max number of groups / number of groups at start 
    UnionFind groups_uf; // a union-find ds of groupIDs.
    // playerID_to_player - hashtable //TODO
    group* groups; // array of size k+1, of struct group s.


public:
    PlayersManager(int k, int scale);
    void Quit();

    StatusType AddPlayer(int PlayerID, int GroupID, int score);

    void print();

};

// public funcs:
PlayersManager::PlayersManager(int k, int scale): k(k), scale(scale), groups_uf(k) {
    groups = new group[k+1];
    for(int i = 0; i < k+1; i++) {
        groups[i].size = 0;
        groups[i].scores = new Players_Ranked_Score[scale+1];
        for(int j = 0; j < scale+1; j++) {
            groups[i].scores[j].score = j;
        }
    }
}

void PlayersManager::Quit() {
    for(int i = 0; i < k+1; i++) {
        delete[] groups[i].scores;
    }
    delete[] groups;
}

StatusType PlayersManager::AddPlayer(int PlayerID, int GroupID, int score) {
    if(PlayerID <= 0 || GroupID <= 0 || GroupID > k || score <= 0 || score > scale) return INVALID_INPUT;

    //TODO: use playerID_to_player to check if player is already in the game

    int actual_groupID = GroupID; //TODO: update this to use union-find.

    

    return SUCCESS;
}


void PlayersManager::print() {
    cout<<"k = " << k << " | scale = " << scale << " | num of players: " << groups[0].players_Ranked_LevelsSum.getNumberOfNodes() <<endl;

}

// private funcs:




#endif // PLAYERS_MANAGER_H