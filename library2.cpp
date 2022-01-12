#include "library2.h"
#include "PlayersManager.h"
#include <iostream>


void* Init(int k, int scale) {
    if(k <= 0 || scale <= 0 || scale > 200) return NULL;
    PlayersManager *DS = new PlayersManager(k, scale);
    return (void*)DS;
}

StatusType AddPlayer(void *DS, int PlayerID, int GroupID, int score) {
    if(DS == NULL) return INVALID_INPUT;
    return ((PlayersManager*)DS)->AddPlayer(PlayerID, GroupID, score);
}

StatusType RemovePlayer(void *DS, int PlayerID) {
    if(DS == NULL) return INVALID_INPUT;
    return ((PlayersManager*)DS)->RemovePlayer(PlayerID);
}

StatusType IncreasePlayerIDLevel(void *DS, int PlayerID, int LevelIncrease) {
    if(DS == NULL) return INVALID_INPUT;
    return ((PlayersManager*)DS)->IncreasePlayerIDLevel(PlayerID, LevelIncrease);
}

StatusType AverageHighestPlayerLevelByGroup(void *DS, int GroupID, int m, double * level) {
    if(DS == NULL) return INVALID_INPUT;
    return ((PlayersManager*)DS)->AverageHighestPlayerLevelByGroup(GroupID, m, level);
}

StatusType MergeGroups(void *DS, int GroupID1, int GroupID2) {
    if(DS == NULL) return INVALID_INPUT;
    return ((PlayersManager*)DS)->MergeGroups(GroupID1, GroupID2);
}

StatusType ChangePlayerIDScore(void *DS, int PlayerID, int NewScore) {
    if(DS == NULL) return INVALID_INPUT;
    return ((PlayersManager*)DS)->ChangePlayerIDScore(PlayerID, NewScore);
}

StatusType GetPercentOfPlayersWithScoreInBounds(void *DS, int GroupID, int score, int lowerLevel, int higherLevel, double * players) {
    if(DS == NULL) return INVALID_INPUT;
    return ((PlayersManager*)DS)->GetPercentOfPlayersWithScoreInBounds(GroupID, score, lowerLevel, higherLevel, players);
}


void Quit(void** DS) {
    ((PlayersManager*)*DS)-> Quit();
    delete ((PlayersManager*)*DS);
    *DS = nullptr;
}

StatusType GetPlayersBound(void *DS, int GroupID, int score, int m, int * LowerBoundPlayers, int * HigherBoundPlayers) {
    if(DS == NULL) return INVALID_INPUT;
    else return FAILURE;
}


//TODO: delete before submission:
void Print(void *DS) {
    ((PlayersManager*)DS)->print();
}


