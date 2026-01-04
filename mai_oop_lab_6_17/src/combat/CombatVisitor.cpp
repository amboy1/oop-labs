#include "combat/CombatVisitor.h"
#include <iostream>

bool Outlaw::accept(CombatVisitor* visitor, NPC* other) {
    return visitor->visit(this, other);
}

bool Knight::accept(CombatVisitor* visitor, NPC* other) {
    return visitor->visit(this, other);
}

bool Elf::accept(CombatVisitor* visitor, NPC* other) {
    return visitor->visit(this, other);
}

bool CombatEngine::visit(Outlaw* self, NPC* other) {
    if (!isInRange(self, other)) return false;
    if (other->getType() == "Elf") {
        subject->notify(self->getName(), other->getName());
        return true;
    }
    return false;
}

bool CombatEngine::visit(Knight* self, NPC* other) {
    if (!isInRange(self, other)) return false;
    if (other->getType() == "Outlaw") {
        subject->notify(self->getName(), other->getName());
        return true;
    }
    return false;
}

bool CombatEngine::visit(Elf* self, NPC* other) {
    if (!isInRange(self, other)) return false;
    if (other->getType() == "Knight") {
        subject->notify(self->getName(), other->getName());
        return true;
    }
    return false;
}