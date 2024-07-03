#include "player.h"
#include <iostream>

Player::Player(const std::string& name) : name(name), victoryPoints(0), towns(0), cities(0), roads(0) {
    resources = { {"Wood", 0}, {"Brick", 0}, {"Sheep", 0}, {"Wheat", 0}, {"Ore", 0} };
}

void Player::collectResource(const std::string& resource, int amount) {
    if (resources.find(resource) != resources.end()) {
        resources[resource] += amount;
    }
}

void Player::printResources() const {
    std::cout << name << "'s resources: ";
    for (const auto& pair : resources) {
        std::cout << pair.first << ": " << pair.second << " ";
    }
    std::cout << "Victory Points: " << victoryPoints << " Towns: " << towns << " Cities: " << cities << " Roads: " << roads << std::endl;
}

std::string Player::getName() const {
    return name;
}

void Player::addVictoryPoints(int points) {
    victoryPoints += points;
}

int Player::getVictoryPoints() const {
    return victoryPoints;
}

void Player::addTown(int vertex) {
    towns++;
    townVertices.insert(vertex);
    addVictoryPoints(1); // Each town is worth 1 victory point
}

void Player::addCity(int vertex) {
    cities++;
    cityVertices.insert(vertex);
    townVertices.erase(vertex); // Remove the town as it is replaced by the city
    addVictoryPoints(1); // Each city gives an additional 1 victory point
}

void Player::addRoad(int vertex1, int vertex2) {
    roads++;
    roadEdges.insert({std::min(vertex1, vertex2), std::max(vertex1, vertex2)});
}

int Player::getTownCount() const {
    return towns;
}

int Player::getCityCount() const {
    return cities;
}

int Player::getRoadCount() const {
    return roads;
}

bool Player::canBuyTown() const {
    return resources.at("Brick") > 0 && resources.at("Wheat") > 0 && resources.at("Wood") > 0 && resources.at("Sheep") > 0;
}

bool Player::canBuildCity() const {
    return resources.at("Ore") >= 3 && resources.at("Wheat") >= 2;
}

bool Player::canBuildRoad() const {
    return resources.at("Brick") > 0 && resources.at("Wood") > 0;
}

void Player::buyTown(int vertex) {
    if (canBuyTown()) {
        resources["Brick"]--;
        resources["Wheat"]--;
        resources["Wood"]--;
        resources["Sheep"]--;
        addTown(vertex);
        std::cout << name << " bought a town at vertex " << vertex << "!" << std::endl;
    } else {
        std::cout << name << " does not have enough resources to buy a town." << std::endl;
    }
}

void Player::buildCity(int vertex) {
    if (canBuildCity()) {
        resources["Ore"] -= 3;
        resources["Wheat"] -= 2;
        addCity(vertex);
        std::cout << name << " built a city at vertex " << vertex << "!" << std::endl;
    } else {
        std::cout << name << " does not have enough resources to build a city." << std::endl;
    }
}

void Player::buildRoad(int vertex1, int vertex2) {
    if (canBuildRoad()) {
        resources["Brick"]--;
        resources["Wood"]--;
        addRoad(vertex1, vertex2);
        std::cout << name << " built a road from vertex " << vertex1 << " to vertex " << vertex2 << "!" << std::endl;
    } else {
        std::cout << name << " does not have enough resources to build a road." << std::endl;
    }
}

const std::set<int>& Player::getTownVertices() const {
    return townVertices;
}

void Player::reduceResourcesByHalf() {
    for (auto& resource : resources) {
        resource.second /= 2;
    }
}
