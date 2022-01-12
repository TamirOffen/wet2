
#ifndef PLAYERS_MANAGER_H
#define PLAYERS_MANAGER_H

#include <iostream>
using namespace std;

#include "Player.h"
#include "unionfind.h"
#include "players_ranked_levelsSum.h"
#include "players_ranked_score.h"
#include "hash.h"


struct Score_Data {
    int num_level0_score;
    Players_Ranked_Score player_ranked_score; // array of size scale+1, of Players_Ranked_Score trees.
};
// an element inside of the groups array in PM.
struct group {
    int size;
    int num_level0_group;
    Players_Ranked_LevelsSum players_Ranked_LevelsSum;
    Score_Data* score_data; // array of size scale+1, of Players_Ranked_Score trees.
};

class PlayersManager {
    int k; // the max number of groups / number of groups at start 
    int scale; // for every player: 1 <= player.score <= scale <= 200.
    UnionFind groups_uf; // a union-find ds of groupIDs.
    HashTable<Player> playerID_to_player;
    group* groups; // array of size k+1, of struct group s.
public:
    PlayersManager(int k, int scale);
    void Quit();
    StatusType AddPlayer(int PlayerID, int GroupID, int score);
    StatusType RemovePlayer(int PlayerID);
    StatusType IncreasePlayerIDLevel(int PlayerID, int LevelIncrease);
    StatusType AverageHighestPlayerLevelByGroup(int GroupID, int m, double * level);
    StatusType MergeGroups(int GroupID1, int GroupID2);
    StatusType ChangePlayerIDScore(int PlayerID, int NewScore);
    StatusType GetPercentOfPlayersWithScoreInBounds(int GroupID, int score, int lowerLevel, int higherLevel, double * players);
    void print();
};

// public funcs: //TODO: playerID_to_player size????
PlayersManager::PlayersManager(int k, int scale): k(k), scale(scale), groups_uf(k), playerID_to_player(997) {
    groups = new group[k+1];
    for(int i = 0; i < k+1; i++) {
        groups[i].size = 0;
        groups[i].num_level0_group = 0;
        groups[i].score_data = new Score_Data[scale+1];
        for(int j = 0; j < scale+1; j++) {
            groups[i].score_data[j].num_level0_score = 0;
            groups[i].score_data[j].player_ranked_score.score = j;
        }
    }
}

void PlayersManager::Quit() {
    for(int i = 0; i < k+1; i++) {
        delete[] groups[i].score_data;
    }
    delete[] groups;
}

StatusType PlayersManager::AddPlayer(int PlayerID, int GroupID, int score) {
    if(PlayerID <= 0 || GroupID <= 0 || GroupID > k || score <= 0 || score > scale) return INVALID_INPUT;

    //use playerID_to_player to check if player is already in the game
    if(playerID_to_player.isInTable(PlayerID) == true) {
        return FAILURE;
    }

    int actual_groupID = groups_uf.find(GroupID); // gets the actual groupID from the uf

    Player newPlayer(PlayerID, GroupID, score); // creates a new player

    playerID_to_player.insert(PlayerID, newPlayer); // adds new player to the hash table

    groups[0].size++;
    groups[actual_groupID].size++;

    groups[0].num_level0_group++;
    groups[actual_groupID].num_level0_group++;

    groups[0].score_data[score].num_level0_score++;
    groups[actual_groupID].score_data[score].num_level0_score++;

    return SUCCESS;
}

StatusType PlayersManager::RemovePlayer(int PlayerID) {
    if(PlayerID <= 0) return INVALID_INPUT;
    if(playerID_to_player.isInTable(PlayerID) == false) return FAILURE;

    Player player = playerID_to_player.find(PlayerID);
    int actual_groupID = groups_uf.find(player.getGroupID());
    int score = player.getScore();

    groups[0].size--;
    groups[actual_groupID].size--;

    if(player.getLevel() == 0) {
        groups[0].num_level0_group--;
        groups[actual_groupID].num_level0_group--;
        groups[0].score_data[score].num_level0_score--;
        groups[actual_groupID].score_data[score].num_level0_score--;
        return SUCCESS;
    }

    groups[0].players_Ranked_LevelsSum.removeNode(player);
    groups[actual_groupID].players_Ranked_LevelsSum.removeNode(player);

    for(int i = 1; i <= scale; i++) {
        groups[0].score_data[i].player_ranked_score.removeNode(player);
        groups[actual_groupID].score_data[i].player_ranked_score.removeNode(player);
    }

    playerID_to_player.remove(PlayerID);

    return SUCCESS;
}

StatusType PlayersManager::IncreasePlayerIDLevel(int PlayerID, int LevelIncrease) {
    if(PlayerID <= 0 || LevelIncrease <= 0) {
        return INVALID_INPUT;
    }

    if(playerID_to_player.isInTable(PlayerID) == false) {
        return FAILURE;
    }

    Player player = playerID_to_player.find(PlayerID);
    int actual_groupID = groups_uf.find(player.getGroupID());

    int newLevel = player.getLevel() + LevelIncrease;
    int prevLevel = player.getLevel();
    int score = player.getScore();

    if(prevLevel == 0) {
        // update num_level0_* fields, because the player is no longer at level 0.
        groups[0].num_level0_group--;
        groups[actual_groupID].num_level0_group--;
        groups[0].score_data[score].num_level0_score--;
        groups[actual_groupID].score_data[score].num_level0_score--;
    }

    // remove the player from the trees:
    groups[0].players_Ranked_LevelsSum.removeNode(player);
    groups[actual_groupID].players_Ranked_LevelsSum.removeNode(player);
    for(int i = 1; i <= scale; i++) {
        groups[0].score_data[i].player_ranked_score.removeNode(player);
        groups[actual_groupID].score_data[i].player_ranked_score.removeNode(player);
    }

    playerID_to_player.remove(PlayerID);
    player.changeLevel(newLevel);
    playerID_to_player.insert(PlayerID, player);
    

    // add the player, with the updated level, back to the trees:
    PlayerInfo info;
    PlayerScoreInfo info2;
    groups[0].players_Ranked_LevelsSum.addNode(player, info);
    groups[actual_groupID].players_Ranked_LevelsSum.addNode(player, info);
    for(int i = 1; i <= scale; i++) {
        groups[0].score_data[i].player_ranked_score.addNode(player, info2);
        groups[actual_groupID].score_data[i].player_ranked_score.addNode(player, info2);
    }


    return SUCCESS;
}

StatusType PlayersManager::AverageHighestPlayerLevelByGroup(int GroupID, int m, double * level) {
    if(GroupID < 0 || GroupID > k || m <= 0) {
        return INVALID_INPUT;
    }

    int actual_groupID = 0;
    if(GroupID != 0) {
        actual_groupID = groups_uf.find(GroupID);
    }

    int n = groups[actual_groupID].size;
    if(n < m) {
        return FAILURE;
    }


    int k_1 = groups[actual_groupID].players_Ranked_LevelsSum.getNumberOfNodes()-m;

    *level = (double)((double)groups[actual_groupID].players_Ranked_LevelsSum.getTotalLevels() 
    - (double)groups[actual_groupID].players_Ranked_LevelsSum.sumTo(groups[actual_groupID].players_Ranked_LevelsSum.select(k_1))) / (double)m;

    return SUCCESS;


}

StatusType PlayersManager::MergeGroups(int GroupID1, int GroupID2) {
    if(GroupID1 <= 0 || GroupID2 <= 0 || GroupID1 > k || GroupID2 > k) {
        return INVALID_INPUT;
    }
    int actual_groupID1 = groups_uf.find(GroupID1);
    int actual_groupID2 = groups_uf.find(GroupID2);
    if(actual_groupID1 == actual_groupID2) {
        return SUCCESS;
    }

    groups_uf.union_groups(actual_groupID1, actual_groupID2);
    int new_groupID = groups_uf.find(actual_groupID1);

    groups[new_groupID].size = groups[actual_groupID1].size + groups[actual_groupID2].size;
    groups[new_groupID].num_level0_group = groups[actual_groupID1].num_level0_group + groups[actual_groupID2].num_level0_group;

    if(new_groupID == actual_groupID1) {
        groups[new_groupID].players_Ranked_LevelsSum.mergeTree(groups[actual_groupID2].players_Ranked_LevelsSum);
        for(int i = 1; i <= scale; i++) {
            groups[new_groupID].score_data[i].num_level0_score += groups[actual_groupID2].score_data[i].num_level0_score;
            groups[new_groupID].score_data[i].player_ranked_score.mergeTree(groups[actual_groupID2].score_data[i].player_ranked_score);
        }

    } else { //new_groupID = actual_groupID2
        groups[new_groupID].players_Ranked_LevelsSum.mergeTree(groups[actual_groupID1].players_Ranked_LevelsSum);
        for(int i = 1; i <= scale; i++) {
            groups[new_groupID].score_data[i].num_level0_score += groups[actual_groupID1].score_data[i].num_level0_score;
            groups[new_groupID].score_data[i].player_ranked_score.mergeTree(groups[actual_groupID1].score_data[i].player_ranked_score);
        }
    }

    return SUCCESS;
}

StatusType PlayersManager::ChangePlayerIDScore(int PlayerID, int NewScore) {
    if(PlayerID <= 0 || NewScore <= 0 || NewScore > scale) {
        return INVALID_INPUT;
    }

    if(playerID_to_player.isInTable(PlayerID) == false) {
        return FAILURE;
    }

    Player player = playerID_to_player.find(PlayerID);
    int actual_groupID = groups_uf.find(player.getGroupID());
    int prevScore = player.getScore();

    if(player.getLevel() == 0) {
        groups[0].score_data[prevScore].num_level0_score--;
        groups[actual_groupID].score_data[prevScore].num_level0_score--;
        groups[0].score_data[NewScore].num_level0_score++;
        groups[actual_groupID].score_data[NewScore].num_level0_score++;
        playerID_to_player.remove(PlayerID);
        player.changeScore(NewScore);
        playerID_to_player.insert(PlayerID, player);
        return SUCCESS;
    }

    // remove the player from the trees:
    groups[0].players_Ranked_LevelsSum.removeNode(player);
    groups[actual_groupID].players_Ranked_LevelsSum.removeNode(player);
    for(int i = 1; i <= scale; i++) {
        groups[0].score_data[i].player_ranked_score.removeNode(player);
        groups[actual_groupID].score_data[i].player_ranked_score.removeNode(player);
    }
    
    playerID_to_player.remove(PlayerID);
    player.changeScore(NewScore);
    playerID_to_player.insert(PlayerID, player);

    // add the player, with the updated score, back to the trees:
    PlayerInfo info;
    PlayerScoreInfo info2;
    groups[0].players_Ranked_LevelsSum.addNode(player, info);
    groups[actual_groupID].players_Ranked_LevelsSum.addNode(player, info);
    for(int i = 1; i <= scale; i++) {
        groups[0].score_data[i].player_ranked_score.addNode(player, info2);
        groups[actual_groupID].score_data[i].player_ranked_score.addNode(player, info2);
    }

    return SUCCESS;

}

StatusType PlayersManager::GetPercentOfPlayersWithScoreInBounds(int GroupID, int score, int lowerLevel, int higherLevel, double* players) {
    if(GroupID < 0 || GroupID > k) return INVALID_INPUT;
    if(score <= 0 || score > scale) return FAILURE;
    int actual_groupID = 0;
    if(GroupID != 0) {
        actual_groupID = groups_uf.find(GroupID);
    }

    int scoreReturn1 = 0, scoreReturn2 = 0;

    // groups[actual_groupID].score_data[score].player_ranked_score
    
    int num_total_players = groups[actual_groupID].score_data[score].player_ranked_score.numOfPlayersLessThan(higherLevel, score, &scoreReturn1) -
         groups[actual_groupID].score_data[score].player_ranked_score.numOfPlayersLessThan(lowerLevel-1, score, &scoreReturn2);

    // cout << num_total_players << 


    if(lowerLevel <= 0) {
        if((num_total_players+groups[actual_groupID].num_level0_group) == 0) {
            return FAILURE;
        }
        *players = (((double)((scoreReturn1 - scoreReturn2)+groups[actual_groupID].score_data[score].num_level0_score) / (num_total_players+groups[actual_groupID].num_level0_group) * 100));
    } else {
        if(num_total_players == 0) {
            return FAILURE;
        }
        *players = (((double)(scoreReturn1 - scoreReturn2) / num_total_players) * 100);
    }

    return SUCCESS;
}

void PlayersManager::print() {
    cout<<"k = " << k << " | scale = " << scale << " | num of total players: " << groups[0].size <<endl;
    for(int i = 0; i <= k; i++) {
        if(i == 0) {
            cout << "All of the players: " << endl;
        }
        else {
            int actual_group = groups_uf.find(i);
            cout <<"GroupID = " << actual_group;
            cout << " | num of players = " << groups[actual_group].size << " | num of level0 player: " << groups[actual_group].num_level0_group <<endl;
        }
    }

}

// private funcs:




#endif // PLAYERS_MANAGER_H