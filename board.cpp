#include "board.h"
#include <iostream>
#include <algorithm>
#include <random>

Board::Board() : thiefTileIndex(-1) {
    initializeBoard();
    initializeVertices();
    initializeThief();
}

void Board::initializeBoard() {
    tiles = {
        { Wood, 5, {0, 1, 2, 3, 4, 5} },
        { Brick, 2, {3, 6, 7, 8, 9, 4} },
        { Sheep, 6, {8, 10, 11, 12, 13, 9} },
        { Wheat, 3, {12, 14, 15, 16, 17, 13} },
        { Ore, 8, {16, 18, 19, 20, 21, 17} },
        { Desert, 7, {20, 22, 23, 24, 25, 21} }
    };
    
    // Shuffle non-desert tiles
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(tiles.begin(), tiles.end() - 1, g);

    // Ensure desert tile is in the center
    tiles.push_back(tiles[5]);
    tiles[5] = { Desert, 7, {20, 22, 23, 24, 25, 21} };
}

void Board::initializeVertices() {
    vertices.resize(26);
    vertexAvailability.resize(26, true);
    vertexOwners.resize(26, ' ');
    for (const auto& tile : tiles) {
        for (int vertex : tile.adjacentVertices) {
            vertices[vertex].push_back(tile.resource);
        }
    }
}

void Board::initializeThief() {
    thiefTileIndex = 5; // Desert tile in the center
}

void Board::printBoard() const {
    for (const auto& tile : tiles) {
        std::cout << resourceToString(tile.resource) << " (" << tile.number << ") ";
    }
    std::cout << std::endl;

    for (size_t i = 0; i < vertices.size(); ++i) {
        std::cout << "Vertex " << i << ": ";
        for (int resource : vertices[i]) {
            std::cout << resourceToString(static_cast<Resource>(resource)) << " ";
        }
        std::cout << (vertexAvailability[i] ? "[Available]" : "[Taken]") << std::endl;
    }
}

void Board::drawBoard() const {
    std::cout << "Board layout:\n";
    for (size_t i = 0; i < vertices.size(); ++i) {
        std::cout << "Vertex " << i << " (" << vertexOwners[i] << "): ";
        for (int resource : vertices[i]) {
            std::cout << resourceToString(static_cast<Resource>(resource)) << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Roads:\n";
    for (const auto& road : roads) {
        std::cout << "Road from Vertex " << road.vertex1 << " to Vertex " << road.vertex2 << " owned by Player " << road.owner << std::endl;
    }

    std::cout << "Thief is on tile: " << thiefTileIndex << " (" << resourceToString(tiles[thiefTileIndex].resource) << ")" << std::endl;
}

std::string resourceToString(Board::Resource resource) {
    switch (resource) {
        case Board::Wood: return "Wood";
        case Board::Brick: return "Brick";
        case Board::Sheep: return "Sheep";
        case Board::Wheat: return "Wheat";
        case Board::Ore: return "Ore";
        case Board::Desert: return "Desert";
        default: return "Unknown";
    }
}

const std::vector<Board::Tile>& Board::getTiles() const {
    return tiles;
}

const std::vector<std::vector<int>>& Board::getVertices() const {
    return vertices;
}

bool Board::isVertexAvailable(int vertex) const {
    return vertex >= 0 && vertex < vertexAvailability.size() && vertexAvailability[vertex];
}

bool Board::isRoadAvailable(int vertex1, int vertex2) const {
    for (const auto& road : roads) {
        if ((road.vertex1 == vertex1 && road.vertex2 == vertex2) || (road.vertex1 == vertex2 && road.vertex2 == vertex1)) {
            return false;
        }
    }
    return true;
}

void Board::placeTown(int vertex, char owner) {
    if (isVertexAvailable(vertex)) {
        vertexAvailability[vertex] = false;
        vertexOwners[vertex] = owner;
    } else {
        std::cerr << "Error: Vertex " << vertex << " is already taken or out of bounds." << std::endl;
    }
}

void Board::placeRoad(int vertex1, int vertex2, char owner) {
    if (isRoadAvailable(vertex1, vertex2)) {
        roads.push_back({vertex1, vertex2, owner});
    } else {
        std::cerr << "Error: Road between Vertex " << vertex1 << " and Vertex " << vertex2 << " is already taken or invalid." << std::endl;
    }
}

void Board::moveThief(int tileIndex) {
    if (tileIndex >= 0 && tileIndex < tiles.size()) {
        thiefTileIndex = tileIndex;
    } else {
        std::cerr << "Error: Invalid tile index for thief." << std::endl;
    }
}

int Board::getThiefTile() const {
    return thiefTileIndex;
}

const std::vector<Board::Road>& Board::getRoads() const {
    return roads;
}
