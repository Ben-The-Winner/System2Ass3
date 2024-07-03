#include "game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Game::Game() : currentPlayerIndex(0) {
    initializePlayers();
    std::srand(std::time(0));
}

void Game::initializePlayers() {
    players.emplace_back("Player 1");
    players.emplace_back("Player 2");
    players.emplace_back("Player 3");

    for (auto& player : players) {
        for (int i = 0; i < 2; ++i) {
            int vertex = getPlayerTownPlacement(player);
            player.addTown(vertex);
            board.placeTown(vertex, player.getName()[7]);
            board.drawBoard();
        }
    }
}

void Game::start() {
    board.printBoard();
    for (auto& player : players) {
        player.printResources();
    }
    nextTurn();
}

void Game::nextTurn() {
    Player& currentPlayer = players[currentPlayerIndex];
    std::cout << currentPlayer.getName() << "'s turn." << std::endl;

    int roll = rollDice();
    std::cout << currentPlayer.getName() << " rolled a " << roll << "." << std::endl;

    if (roll == 7) {
        handleThief(currentPlayer);
    } else {
        distributeResources(roll);
    }

    currentPlayer.printResources();

    if (currentPlayer.canBuyTown()) {
        int vertex = getPlayerTownPlacement(currentPlayer);
        if (board.isVertexAvailable(vertex)) {
            currentPlayer.buyTown(vertex);
            board.placeTown(vertex, currentPlayer.getName()[7]);
            board.drawBoard();
        } else {
            std::cout << "Vertex " << vertex << " is already taken. Choose another vertex." << std::endl;
        }
    }

    if (currentPlayer.canBuildCity()) {
        int vertex = getPlayerCityPlacement(currentPlayer);
        if (!board.isVertexAvailable(vertex)) {
            currentPlayer.buildCity(vertex);
            board.placeTown(vertex, currentPlayer.getName()[7]); // Replace town with city
            board.drawBoard();
        } else {
            std::cout << "Vertex " << vertex << " does not have a town. Choose another vertex." << std::endl;
        }
    }

    if (currentPlayer.canBuildRoad()) {
        int vertex1, vertex2;
        std::cout << currentPlayer.getName() << ", choose two vertices to build your road (separated by space): ";
        std::cin >> vertex1 >> vertex2;
        if (board.isRoadAvailable(vertex1, vertex2)) {
            currentPlayer.buildRoad(vertex1, vertex2);
            board.placeRoad(vertex1, vertex2, currentPlayer.getName()[7]);
            board.drawBoard();
            calculateLongestRoad();
        } else {
            std::cout << "Road between " << vertex1 << " and " << vertex2 << " is already taken. Choose another road." << std::endl;
        }
    }

    if (checkVictory()) {
        std::cout << currentPlayer.getName() << " wins with " << currentPlayer.getVictoryPoints() << " victory points!" << std::endl;
        return;
    }

    currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
    nextTurn();
}

bool Game::checkVictory() {
    return players[currentPlayerIndex].getVictoryPoints() >= 10;
}

int Game::rollDice() const {
    return (std::rand() % 6) + 1 + (std::rand() % 6) + 1;
}

void Game::distributeResources(int roll) {
    const std::vector<Board::Tile>& tiles = board.getTiles();
    for (const auto& tile : tiles) {
        if (tile.number == roll && board.getThiefTile() != tile.number) {
            std::string resource = resourceToString(tile.resource);
            for (int vertex : tile.adjacentVertices) {
                for (auto& player : players) {
                    if (player.getTownCount() > 0 && player.getTownVertices().count(vertex)) {
                        player.collectResource(resource);
                    } else if (player.getCityCount() > 0 && player.getTownVertices().count(vertex)) {
                        player.collectResource(resource, 2); // Cities yield double resources
                    }
                }
            }
        }
    }
}

int Game::getPlayerTownPlacement(const Player& player) const {
    int vertex;
    bool valid = false;
    while (!valid) {
        std::cout << player.getName() << ", choose a vertex to place your town: ";
        std::cin >> vertex;
        if (board.isVertexAvailable(vertex)) {
            valid = true;
        } else {
            std::cout << "Vertex " << vertex << " is already taken or invalid. Choose another vertex." << std::endl;
        }
    }
    return vertex;
}

int Game::getPlayerCityPlacement(const Player& player) const {
    int vertex;
    bool valid = false;
    while (!valid) {
        std::cout << player.getName() << ", choose a vertex to upgrade your town to a city: ";
        std::cin >> vertex;
        if (!board.isVertexAvailable(vertex) && player.getTownVertices().count(vertex)) {
            valid = true;
        } else {
            std::cout << "Vertex " << vertex << " does not have your town. Choose another vertex." << std::endl;
        }
    }
    return vertex;
}

void Game::handleThief(Player& player) {
    int tileIndex;
    std::cout << player.getName() << ", choose a tile to move the thief to: ";
    std::cin >> tileIndex;
    board.moveThief(tileIndex);
    std::cout << "Thief moved to tile " << tileIndex << std::endl;

    // Reduce resources for players with towns adjacent to the thief
    for (const auto& adjVertex : board.getTiles()[tileIndex].adjacentVertices) {
        for (auto& player : players) {
            if (player.getTownVertices().count(adjVertex)) {
                player.reduceResourcesByHalf();
            }
        }
    }
}

void Game::calculateLongestRoad() {
    int maxRoads = 0;
    Player* longestRoadPlayer = nullptr;
    for (auto& player : players) {
        if (player.getRoadCount() > maxRoads) {
            maxRoads = player.getRoadCount();
            longestRoadPlayer = &player;
        }
    }

    if (longestRoadPlayer != nullptr) {
        longestRoadPlayer->addVictoryPoints(2);
        std::cout << longestRoadPlayer->getName() << " now has the longest road!" << std::endl;
    }
}
