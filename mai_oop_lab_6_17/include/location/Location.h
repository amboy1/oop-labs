#pragma once
#include "NPC.h"
#include "../factory/NPCFactory.h"
#include "../combat/CombatVisitor.h"
#include "../observer/Observer.h"
#include <vector>
#include <algorithm>
#include <fstream>
#include <stdexcept>

class Location : public Subject {
private:
    std::vector<std::shared_ptr<NPC>> npcs;
    std::shared_ptr<NPCFactory> factory;
    std::string filename = "location.txt";

public:
    Location(std::shared_ptr<NPCFactory> f) : factory(f) {
        std::ofstream log("data/log.txt", std::ios::trunc);
        if (log.is_open()) log.close();
    }

    void addNPC(const std::string& type, int x, int y, const std::string& name);
    void save();
    void load();
    void print() const;
    void startCombat(double range);
};