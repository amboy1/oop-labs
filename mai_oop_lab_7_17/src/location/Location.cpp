#include "location/Location.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <sstream>

Location::Location(std::shared_ptr<NPCFactory> f) 
    : factory(f), gameRunning(false), timeElapsed(0) {
}

Location::~Location() {
    stop();
}

void Location::stop() {
    gameRunning = false;
    combatCV.notify_all();
}

void Location::createRandomNPCs(int count) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> typeDist(0, 2);
    std::uniform_int_distribution<> xDist(0, MAP_WIDTH - 1);
    std::uniform_int_distribution<> yDist(0, MAP_HEIGHT - 1);
    
    std::vector<std::string> types = {"Outlaw", "Knight", "Elf"};
    
    {
        std::unique_lock lock(npcsMutex);
        
        npcs.clear();
        
        for (int i = 0; i < count; ++i) {
            std::string type = types[typeDist(gen)];
            std::string name = type + "_" + std::to_string(i + 1);
            int x = xDist(gen);
            int y = yDist(gen);
            
            try {
                auto npc = factory->createNPC(type, x, y, name);
                npcs.push_back(npc);
            } catch (const std::exception& e) {
                std::lock_guard<std::mutex> coutLock(coutMutex);
                std::cerr << "Ошибка создания NPC: " << e.what() << std::endl;
            }
        }
    }
    
    {
        std::lock_guard<std::mutex> coutLock(coutMutex); 
        std::cout << "Создано " << count << " NPC" << std::endl;
    }
}

void Location::runGame(int durationSeconds) {
    gameRunning = true;
    timeElapsed = 0;
    
    // Запускаем потоки
    movementThread = std::thread(&Location::movementLoop, this);
    combatThread = std::thread(&Location::combatLoop, this);
    printThread = std::thread(&Location::printLoop, this);
    
    // Ждем указанное время
    std::this_thread::sleep_for(std::chrono::seconds(durationSeconds));
    
    // Останавливаем игру
    stop();
    
    // Ждем завершения потоков
    if (movementThread.joinable()) movementThread.join();
    if (combatThread.joinable()) combatThread.join();
    if (printThread.joinable()) printThread.join();
}

void Location::movementLoop() {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    while (gameRunning) {
        // Ждем 100 мс между движениями
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        {
            std::shared_lock lock(npcsMutex);
            
            // Двигаем только живых NPC
            for (auto& npc : npcs) {
                if (npc->isAlive()) {
                    moveNPC(npc);
                }
            }
            
            // Проверяем возможные бои (упрощенная версия)
            // Для эффективности можно использовать пространственные структуры данных
            for (size_t i = 0; i < npcs.size() && gameRunning; ++i) {
                if (!npcs[i]->isAlive()) continue;
                
                for (size_t j = i + 1; j < npcs.size() && gameRunning; ++j) {
                    if (!npcs[j]->isAlive()) continue;
                    
                    if (isInCombatRange(npcs[i], npcs[j])) {
                        checkForCombat(npcs[i], npcs[j]);
                    }
                }
            }
        }
        
        timeElapsed += 100;
    }
}

void Location::moveNPC(std::shared_ptr<NPC> npc) {
    int moveDistance = npc->getMoveDistance();
    if (moveDistance == 0) return;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(-moveDistance, moveDistance);
    
    int newX = npc->getX() + dist(gen);
    int newY = npc->getY() + dist(gen);
    
    // Проверяем границы карты
    newX = std::max(0, std::min(MAP_WIDTH - 1, newX));
    newY = std::max(0, std::min(MAP_HEIGHT - 1, newY));
    
    npc->setPosition(newX, newY);
}

bool Location::isInCombatRange(std::shared_ptr<NPC> npc1, std::shared_ptr<NPC> npc2) {
    int dx = npc1->getX() - npc2->getX();
    int dy = npc1->getY() - npc2->getY();
    double distance = std::sqrt(dx*dx + dy*dy);
    
    return distance <= std::max(npc1->getCombatRange(), npc2->getCombatRange());
}

void Location::checkForCombat(std::shared_ptr<NPC> npc1, std::shared_ptr<NPC> npc2) {
    // Используем CombatEngine для проверки логики боя
    auto combatEngine = std::make_unique<CombatEngine>(
        std::max(npc1->getCombatRange(), npc2->getCombatRange()) * 1.0, 
        this
    );
    
    if (npc1->accept(combatEngine.get(), npc2.get())) {
        // npc1 может атаковать npc2
        CombatTask task{npc1, npc2};
        {
            std::lock_guard<std::mutex> lock(combatQueueMutex);
            combatQueue.push(task);
        }
        combatCV.notify_one();
    }
    
    // Проверяем обратное направление
    if (npc2->accept(combatEngine.get(), npc1.get())) {
        // npc2 может атаковать npc1
        CombatTask task{npc2, npc1};
        {
            std::lock_guard<std::mutex> lock(combatQueueMutex);
            combatQueue.push(task);
        }
        combatCV.notify_one();
    }
}

void Location::combatLoop() {
    while (gameRunning) {
        CombatTask task;
        bool hasTask = false;
        
        {
            std::unique_lock<std::mutex> lock(combatQueueMutex);
            combatCV.wait_for(lock, std::chrono::milliseconds(100), 
                [this]() { return !combatQueue.empty() || !gameRunning; });
            
            if (!combatQueue.empty()) {
                task = combatQueue.front();
                combatQueue.pop();
                hasTask = true;
            }
        }
        
        if (hasTask) {
            // Проверяем, что NPC все еще живы
            if (task.attacker->isAlive() && task.defender->isAlive()) {
                resolveCombat(task.attacker, task.defender);
            }
        }
    }
}

void Location::printLoop() {
    while (gameRunning) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        {
            std::lock_guard<std::mutex> coutLock(coutMutex);
            std::cout << "\n=== Карта (" << timeElapsed / 1000 << " сек) ===" << std::endl;
            
            // Создаем простую текстовую карту
            std::vector<std::vector<char>> map(MAP_HEIGHT, std::vector<char>(MAP_WIDTH, '.'));
            int aliveCount = 0;
            
            {
                std::shared_lock lock(npcsMutex);
                for (const auto& npc : npcs) {
                    if (npc->isAlive()) {
                        int x = npc->getX();
                        int y = npc->getY();
                        
                        if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) {
                            char symbol = '?';
                            std::string type = npc->getType();
                            if (type == "Knight") symbol = 'K';
                            else if (type == "Elf") symbol = 'E';
                            else if (type == "Outlaw") symbol = 'O';
                            
                            map[y][x] = symbol;
                        }
                        aliveCount++;
                    }
                }
            }
            
            // Выводим небольшую часть карты для наглядности
            int displaySize = 20;
            for (int y = 0; y < displaySize && y < MAP_HEIGHT; ++y) {
                for (int x = 0; x < displaySize && x < MAP_WIDTH; ++x) {
                    std::cout << map[y][x] << ' ';
                }
                std::cout << std::endl;
            }
            
            std::cout << "Живых NPC: " << aliveCount << "/" << npcs.size() << std::endl;
        }
    }
}

void Location::printSurvivors() const {
    std::lock_guard<std::mutex> coutLock(coutMutex);
    std::cout << "\n=== СПИСОК ВЫЖИВШИХ ===" << std::endl;
    
    std::shared_lock lock(npcsMutex);
    int count = 0;
    for (const auto& npc : npcs) {
        if (npc->isAlive()) {
            std::cout << ++count << ". ";
            npc->print();
            std::cout << std::endl;
        }
    }
    
    if (count == 0) {
        std::cout << "Нет выживших" << std::endl;
    } else {
        std::cout << "Всего выжило: " << count << " NPC" << std::endl;
    }
}

void Location::resolveCombat(std::shared_ptr<NPC> attacker, std::shared_ptr<NPC> defender) {
    // Создаем отдельный CombatEngine для броска кубика
    CombatEngine diceEngine(0, this);  // Диапазон не важен
    
    // Бросаем кубики для атаки и защиты
    int attackPower = diceEngine.rollDice();  // Атакующий бросает
    int defensePower = diceEngine.rollDice(); // Защищающийся бросает
    
    {
        std::lock_guard<std::mutex> coutLock(coutMutex);
        std::cout << "БОЙ: " << attacker->getName() << " (" << attackPower 
                  << ") vs " << defender->getName() << " (" << defensePower << ")" << std::endl;
    }
    
    // Логика по ТЗ: если сила атаки > силы защиты - убийство
    if (attackPower > defensePower) {
        defender->setAlive(false);
        notify(attacker->getName(), defender->getName());
        
        {
            std::lock_guard<std::mutex> coutLock(coutMutex);
            std::cout << "УБИЙСТВО: " << attacker->getName() 
                      << " убивает " << defender->getName() 
                      << " (" << attackPower << " > " << defensePower << ")" << std::endl;
        }
    } else {
        {
            std::lock_guard<std::mutex> coutLock(coutMutex);
            std::cout << "ЗАЩИТА: " << defender->getName() 
                      << " защитился от " << attacker->getName()
                      << " (" << attackPower << " <= " << defensePower << ")" << std::endl;
        }
    }
}