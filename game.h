#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "player.h"
#include <vector>

class Game {
public:
    Game();
    void start();

private:
    Board board;
    std::vector<Player> players;
    int currentPlayerIndex;
    void initializePlayers();
    void nextTurn();
    bool checkVictory();
    int rollDice() const;
    void distributeResources(int roll);
    int getPlayerTownPlacement(const Player& player) const;
    int getPlayerCityPlacement(const Player& player) const; // Add this line
    void handleThief(Player& player);
    void calculateLongestRoad();
};

#endif // GAME_H
