#pragma once
#include "../location/NPC.h"
#include "../observer/Observer.h"
#include <cmath>
#include <random>

class CombatVisitor {
protected:
    double combatRange;
    Subject* subject;
    
public:
    CombatVisitor(double range, Subject* s) : combatRange(range), subject(s) {}
    virtual ~CombatVisitor() = default;

    bool isInRange(NPC* npc1, NPC* npc2) const {
        double dx = npc1->getX() - npc2->getX();
        double dy = npc1->getY() - npc2->getY();
        return std::sqrt(dx * dx + dy * dy) <= combatRange;
    }

    virtual bool canAttack(Outlaw* self, NPC* other) = 0;
    virtual bool canAttack(Knight* self, NPC* other) = 0;
    virtual bool canAttack(Elf* self, NPC* other) = 0;
};

class CombatEngine : public CombatVisitor {
private:
    std::random_device rd;
    std::mt19937 gen;
    
public:
    CombatEngine(double range, Subject* s);
    
    bool canAttack(Outlaw* self, NPC* other) override;
    bool canAttack(Knight* self, NPC* other) override;
    bool canAttack(Elf* self, NPC* other) override;
    
    int rollDice();
};