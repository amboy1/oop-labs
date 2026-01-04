#pragma once
#include "NPC.h"
#include "../factory/NPCFactory.h"
#include "../combat/CombatVisitor.h"
#include "../observer/Observer.h"
#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <queue>
#include <atomic>
#include <random>
#include <condition_variable>

struct CombatTask {
    std::shared_ptr<NPC> attacker;
    std::shared_ptr<NPC> defender;
};

class Location : public Subject {
private:
    std::vector<std::shared_ptr<NPC>> npcs;
    std::shared_ptr<NPCFactory> factory;
    
    // Многопоточные компоненты
    mutable std::shared_mutex npcsMutex;
    mutable std::mutex coutMutex;                   
    std::mutex combatQueueMutex;
    std::condition_variable combatCV;
    
    std::queue<CombatTask> combatQueue;
    std::atomic<bool> gameRunning;
    std::atomic<int> timeElapsed;
    
    // Потоки
    std::thread movementThread;
    std::thread combatThread;
    std::thread printThread;
    
    // Константы
    static constexpr int MAP_WIDTH = 100;
    static constexpr int MAP_HEIGHT = 100;
    
    // Приватные методы потоков
    void movementLoop();
    void combatLoop();
    void printLoop();
    
    // Вспомогательные методы
    void moveNPC(std::shared_ptr<NPC> npc);
    void checkForCombat(std::shared_ptr<NPC> npc1, std::shared_ptr<NPC> npc2);
    bool isInCombatRange(std::shared_ptr<NPC> npc1, std::shared_ptr<NPC> npc2);
    void resolveCombat(std::shared_ptr<NPC> attacker, std::shared_ptr<NPC> defender);
    void rollDice(int& attack, int& defense);
    
public:
    Location(std::shared_ptr<NPCFactory> f);
    ~Location();
    
    // Основные публичные методы
    void createRandomNPCs(int count);
    void runGame(int durationSeconds);
    void stop(); 
    void printSurvivors() const;
    
    template<typename Func>
    void accessNPCs(Func func) {
        std::shared_lock lock(npcsMutex);
        func(npcs);
    }
    
    size_t getNPCCount() const {
        std::shared_lock lock(npcsMutex);
        return npcs.size();
    }
    
    size_t getAliveNPCCount() const {
        std::shared_lock lock(npcsMutex);
        size_t count = 0;
        for (const auto& npc : npcs) {
            if (npc->isAlive()) count++;
        }
        return count;
    }
};