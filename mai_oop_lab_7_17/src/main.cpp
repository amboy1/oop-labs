#include "location/Location.h"
#include "factory/NPCFactory.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <memory>

using namespace std;

int main() {
    // Создаем фабрику и локацию
    shared_ptr<NPCFactory> factory = make_shared<BalagurFateNPCFactory>();
    Location location(factory);
    
    // Создаем случайных NPC
    location.createRandomNPCs(50);
    
    cout << "=== Игра началась! 50 NPC создано. ===" << endl;
    cout << "Игра закончится через 30 секунд..." << endl;
    
    // Запускаем игровой цикл
    location.runGame(30);
    
    // Выводим статистику
    location.printSurvivors();
    
    return 0;
}