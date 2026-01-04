#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include <atomic>

class CombatVisitor;

class NPC {
protected:
    int x, y;
    std::string name;
    std::atomic<bool> alive;
    int combatRange;
    int moveDistance;

public:
    NPC(int x, int y, const std::string& name, int combatRange, int moveDistance) 
        : x(x), y(y), name(name), alive(true), combatRange(combatRange), moveDistance(moveDistance) {}
    
    virtual ~NPC() = default;

    int getX() const { return x; }
    int getY() const { return y; }
    const std::string& getName() const { return name; }
    bool isAlive() const { return alive.load(); }
    int getCombatRange() const { return combatRange; }
    int getMoveDistance() const { return moveDistance; }
    
    void setPosition(int newX, int newY) {
        x = newX;
        y = newY;
    }
    
    void setAlive(bool isAlive) { alive.store(isAlive); }

    virtual std::string getType() const = 0;

    virtual std::string serialize() const {
        std::stringstream ss;
        ss << getType() << " " << x << " " << y << " " << name;
        return ss.str();
    }

    virtual bool accept(CombatVisitor* visitor, NPC* other) = 0;

    virtual void print() const {
        std::cout << getType() << " (" << name << "): [" << x << ", " << y << "]";
        if (!isAlive()) std::cout << " [МЕРТВ]";
    }
};

class Outlaw : public NPC { 
public:
    Outlaw(int x, int y, const std::string& name) 
        : NPC(x, y, name, 10, 10) {}
    std::string getType() const override { return "Outlaw"; }
    bool accept(CombatVisitor* visitor, NPC* other) override;
};

class Knight : public NPC { 
public:
    Knight(int x, int y, const std::string& name) 
        : NPC(x, y, name, 10, 30) {}
    std::string getType() const override { return "Knight"; }
    bool accept(CombatVisitor* visitor, NPC* other) override;
};

class Elf : public NPC { 
public:
    Elf(int x, int y, const std::string& name) 
        : NPC(x, y, name, 50, 10) {}
    std::string getType() const override { return "Elf"; }
    bool accept(CombatVisitor* visitor, NPC* other) override;
};