#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <sstream>

class CombatVisitor;

class NPC {
protected:
    int x, y;
    std::string name;

public:
    NPC(int x, int y, const std::string& name) : x(x), y(y), name(name) {}
    virtual ~NPC() = default;

    int getX() const { return x; }
    int getY() const { return y; }
    const std::string& getName() const { return name; }

    virtual std::string getType() const = 0;

    virtual std::string serialize() const {
        std::stringstream ss;
        ss << getType() << " " << x << " " << y << " " << name;
        return ss.str();
    }

    virtual bool accept(CombatVisitor* visitor, NPC* other) = 0;

    void print() const {
        std::cout << getType() << " (" << name << "): [" << x << ", " << y << "]";
    }
};

class Outlaw : public NPC { 
public:
    Outlaw(int x, int y, const std::string& name) : NPC(x, y, name) {}
    std::string getType() const override { return "Outlaw"; }
    bool accept(CombatVisitor* visitor, NPC* other) override;
};

class Knight : public NPC { 
public:
    Knight(int x, int y, const std::string& name) : NPC(x, y, name) {}
    std::string getType() const override { return "Knight"; }
    bool accept(CombatVisitor* visitor, NPC* other) override;
};

class Elf : public NPC { 
public:
    Elf(int x, int y, const std::string& name) : NPC(x, y, name) {}
    std::string getType() const override { return "Elf"; }
    bool accept(CombatVisitor* visitor, NPC* other) override;
};