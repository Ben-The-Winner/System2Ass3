#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <map>
#include <string>

class Board {
public:
    enum Resource { Wood, Brick, Sheep, Wheat, Ore, Desert };
    struct Tile {
        Resource resource;
        int number;
        std::vector<int> adjacentVertices;
    };

    struct Road {
        int vertex1;
        int vertex2;
        char owner;
    };

    Board();
    void printBoard() const;
    void drawBoard() const;
    const std::vector<Tile>& getTiles() const;
    const std::vector<std::vector<int>>& getVertices() const;
    bool isVertexAvailable(int vertex) const;
    bool isRoadAvailable(int vertex1, int vertex2) const;
    void placeTown(int vertex, char owner);
    void placeRoad(int vertex1, int vertex2, char owner);
    void moveThief(int tileIndex);
    int getThiefTile() const;
    const std::vector<Road>& getRoads() const; // Ensure this is declared

private:
    std::vector<Tile> tiles;
    std::vector<std::vector<int>> vertices;
    std::vector<bool> vertexAvailability;
    std::vector<char> vertexOwners;
    std::vector<Road> roads;
    int thiefTileIndex;
    void initializeBoard();
    void initializeVertices();
    void initializeThief();
};

std::string resourceToString(Board::Resource resource);

#endif // BOARD_H
