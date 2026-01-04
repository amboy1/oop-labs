#pragma once
#include "../location/NPC.h"
#include "../observer/Observer.h"
#include <cmath>

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

    virtual bool visit(Outlaw* self, NPC* other) = 0;
    virtual bool visit(Knight* self, NPC* other) = 0;
    virtual bool visit(Elf* self, NPC* other) = 0;
};

class CombatEngine : public CombatVisitor {
public:
    CombatEngine(double range, Subject* s) : CombatVisitor(range, s) {}

    bool visit(Outlaw* self, NPC* other) override;
    bool visit(Knight* self, NPC* other) override;
    bool visit(Elf* self, NPC* other) override;
};