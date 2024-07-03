#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <map>
#include <set>

class Player {
public:
    Player(const std::string& name);

    void collectResource(const std::string& resource, int amount = 1); // Add amount parameter
    void printResources() const;
    std::string getName() const;
    void addVictoryPoints(int points);
    int getVictoryPoints() const;
    void addTown(int vertex);
    void addCity(int vertex); // Add this method
    void addRoad(int vertex1, int vertex2);
    int getTownCount() const;
    int getCityCount() const; // Add this method
    int getRoadCount() const;
    bool canBuyTown() const;
    bool canBuildCity() const; // Add this method
    bool canBuildRoad() const;
    void buyTown(int vertex);
    void buildCity(int vertex); // Add this method
    void buildRoad(int vertex1, int vertex2);
    const std::set<int>& getTownVertices() const;
    void reduceResourcesByHalf();

private:
    std::string name;
    std::map<std::string, int> resources;
    int victoryPoints;
    int towns;
    int cities; // Add this member
    int roads;
    std::set<int> townVertices;
    std::set<int> cityVertices; // Add this member
    std::set<std::pair<int, int>> roadEdges;
};

#endif // PLAYER_H
