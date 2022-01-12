#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
using namespace std;


class Player {
    int playerID;
    int groupID;
    int level;
    int score;

public:
    // default constructor
    Player(): 
        playerID(-1), groupID(-1), level(0), score(-1) {};

    // creates a new player, starting at level 0
    Player(int playerID, int groupID, int score): 
        playerID(playerID), groupID(groupID), level(0), score(score) {};

    // changes the level of the player
    void changeLevel(int newLevel) {
        level = newLevel;
    }

    // changes the score of the player
    void changeScore(int newScore) {
        score = newScore;
    }

    int getLevel() {
        return level;
    }
    int getScore() {
        return score;
    }
    int getGroupID() {
        return groupID;
    }
    int getPlayerID() {
        return playerID;
    }
    // compare funcs:
    friend bool operator==(const Player& lhs, const Player& rhs) {
        return lhs.level == rhs.level && lhs.playerID == rhs.playerID;
    }
    friend bool operator<(const Player& lhs, const Player& rhs) {
        if(lhs.level < rhs.level) return true;
        if(lhs.level == rhs.level) {
            return (lhs.playerID < rhs.playerID);
        } else {
            return false;
        }

    }
    friend bool operator>(const Player& lhs, const Player& rhs) {
        if(lhs == rhs) return false;
        return !(lhs < rhs);
    }

    // for testing:
    friend ostream& operator<<(ostream& os, Player const & player) {
        // return os << "(" << player.playerID << "," << player.groupID << "," << player.level << "," << player.score << ")" << endl;
        // return os << "(" << player.playerID << "," << player.level << ")";
        return os << "(pID: " << player.playerID << ", score: " << player.score << ", level " << player.level << ")";
    }

};


#endif


