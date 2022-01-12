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


void Quit(void** DS) {
    ((PlayersManager*)*DS)-> Quit();
    delete ((PlayersManager*)*DS);
    *DS = nullptr;
}


//TODO: delete before submission:
void Print(void *DS) {
    ((PlayersManager*)DS)->print();
}


