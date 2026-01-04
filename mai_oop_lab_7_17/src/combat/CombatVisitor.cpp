#include "combat/CombatVisitor.h"
#include <iostream>

// Конструктор
CombatEngine::CombatEngine(double range, Subject* s) 
    : CombatVisitor(range, s), gen(rd()) {}

// Методы accept для NPC
bool Outlaw::accept(CombatVisitor* visitor, NPC* other) {
    auto combatEngine = dynamic_cast<CombatEngine*>(visitor);
    if (combatEngine) {
        return combatEngine->canAttack(this, other);
    }
    return false;
}

bool Knight::accept(CombatVisitor* visitor, NPC* other) {
    auto combatEngine = dynamic_cast<CombatEngine*>(visitor);
    if (combatEngine) {
        return combatEngine->canAttack(this, other);
    }
    return false;
}

bool Elf::accept(CombatVisitor* visitor, NPC* other) {
    auto combatEngine = dynamic_cast<CombatEngine*>(visitor);
    if (combatEngine) {
        return combatEngine->canAttack(this, other);
    }
    return false;
}

// Методы canAttack для CombatEngine
bool CombatEngine::canAttack(Outlaw* self, NPC* other) {
    if (!isInRange(self, other)) return false;
    return other->getType() == "Elf";  // Разбойник может атаковать эльфа
}

bool CombatEngine::canAttack(Knight* self, NPC* other) {
    if (!isInRange(self, other)) return false;
    return other->getType() == "Outlaw";  // Рыцарь может атаковать разбойника
}

bool CombatEngine::canAttack(Elf* self, NPC* other) {
    if (!isInRange(self, other)) return false;
    return other->getType() == "Knight";  // Эльф может атаковать рыцаря
}

// Метод для броска кубика
int CombatEngine::rollDice() {
    std::uniform_int_distribution<> dice(1, 6);
    return dice(gen);
}