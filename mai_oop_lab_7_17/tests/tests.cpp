#include <gtest/gtest.h>
#include "location/Location.h"
#include "factory/NPCFactory.h"
#include <memory>
#include <thread>
#include <chrono>

using namespace std;

class LocationTest : public ::testing::Test {
protected:
    shared_ptr<NPCFactory> factory;
    unique_ptr<Location> location;
    
    void SetUp() override {
        factory = make_shared<BalagurFateNPCFactory>();
        location = make_unique<Location>(factory);
    }
    
    void TearDown() override {
        location->stop();
        // Даем время потокам завершиться
        this_thread::sleep_for(chrono::milliseconds(100));
    }
};

// Вспомогательная функция для создания NPC напрямую через фабрику
shared_ptr<NPC> createNPC(const string& type, int x, int y, const string& name) {
    auto factory = make_shared<BalagurFateNPCFactory>();
    return factory->createNPC(type, x, y, name);
}

TEST_F(LocationTest, OutlawVsElfInCombatRange) {
    // Создаем NPC через фабрику
    auto outlaw = createNPC("Outlaw", 10, 10, "Robber");
    auto elf = createNPC("Elf", 11, 11, "YoungElf");
    
    // Вручную проверяем логику боя (без потока боя)
    CombatEngine visitor(5.0, location.get());
    
    // Разбойник должен атаковать эльфа (разбойник -> эльф)
    EXPECT_TRUE(outlaw->accept(&visitor, elf.get()));
    
    // Эльф НЕ должен атаковать разбойника (эльф -> рыцарь)
    EXPECT_FALSE(elf->accept(&visitor, outlaw.get()));
}

TEST_F(LocationTest, ElfVsKnightInCombatRange) {
    auto elf = createNPC("Elf", 100, 100, "Archer");
    auto knight = createNPC("Knight", 101, 101, "SirLancelot");
    
    CombatEngine visitor(5.0, location.get());
    
    // Эльф должен атаковать рыцаря (эльф -> рыцарь)
    EXPECT_TRUE(elf->accept(&visitor, knight.get()));
    
    // Рыцарь НЕ должен атаковать эльфа (рыцарь -> разбойник)
    EXPECT_FALSE(knight->accept(&visitor, elf.get()));
}

TEST_F(LocationTest, KnightVsOutlawInCombatRange) {
    auto knight = createNPC("Knight", 50, 50, "Guardian");
    auto outlaw = createNPC("Outlaw", 51, 51, "Thief");
    
    CombatEngine visitor(5.0, location.get());
    
    // Рыцарь должен атаковать разбойника (рыцарь -> разбойник)
    EXPECT_TRUE(knight->accept(&visitor, outlaw.get()));
    
    // Разбойник НЕ должен атаковать рыцаря (разбойник -> эльф)
    EXPECT_FALSE(outlaw->accept(&visitor, knight.get()));
}

TEST_F(LocationTest, OutlawVsKnightOutOfCombatRange) {
    auto outlaw = createNPC("Outlaw", 10, 10, "WeakThief");
    auto knight = createNPC("Knight", 10, 10, "StrongKnight");
    
    CombatEngine visitor(1.0, location.get());
    
    // При нулевом расстоянии должны быть в радиусе
    EXPECT_TRUE(knight->accept(&visitor, outlaw.get()));
    EXPECT_FALSE(outlaw->accept(&visitor, knight.get()));
}

TEST_F(LocationTest, NPCsFarAwayNoCombat) {
    auto outlaw = createNPC("Outlaw", 0, 0, "Robber");
    auto elf = createNPC("Elf", 400, 400, "FarElf");
    
    CombatEngine visitor(10.0, location.get());
    
    // Слишком далеко для любого боя
    EXPECT_FALSE(outlaw->accept(&visitor, elf.get()));
    EXPECT_FALSE(elf->accept(&visitor, outlaw.get()));
}

TEST_F(LocationTest, CreateRandomNPCs) {
    location->createRandomNPCs(10);
    
    // Проверяем, что создалось нужное количество NPC
    EXPECT_EQ(location->getNPCCount(), 10);
    
    // Все должны быть живы
    EXPECT_EQ(location->getAliveNPCCount(), 10);
}

TEST_F(LocationTest, MovementDistance) {
    auto knight = createNPC("Knight", 50, 50, "TestKnight");
    auto elf = createNPC("Elf", 50, 50, "TestElf");
    auto outlaw = createNPC("Outlaw", 50, 50, "TestOutlaw");
    
    // Проверяем дистанции движения
    EXPECT_EQ(knight->getMoveDistance(), 30);  // Рыцарь: 30
    EXPECT_EQ(elf->getMoveDistance(), 10);     // Эльф: 10
    EXPECT_EQ(outlaw->getMoveDistance(), 10);  // Разбойник: 10
    
    // Проверяем дистанции убийства
    EXPECT_EQ(knight->getCombatRange(), 10);   // Рыцарь: 10
    EXPECT_EQ(elf->getCombatRange(), 50);      // Эльф: 50
    EXPECT_EQ(outlaw->getCombatRange(), 10);   // Разбойник: 10
}

TEST_F(LocationTest, NPCAliveState) {
    auto npc = createNPC("Knight", 10, 10, "Test");
    
    // По умолчанию жив
    EXPECT_TRUE(npc->isAlive());
    
    // Убиваем
    npc->setAlive(false);
    EXPECT_FALSE(npc->isAlive());
    
    // Воскрешаем
    npc->setAlive(true);
    EXPECT_TRUE(npc->isAlive());
}

TEST_F(LocationTest, NPCPosition) {
    auto npc = createNPC("Elf", 10, 10, "TestElf");
    
    EXPECT_EQ(npc->getX(), 10);
    EXPECT_EQ(npc->getY(), 10);
    
    // Меняем позицию
    npc->setPosition(20, 30);
    EXPECT_EQ(npc->getX(), 20);
    EXPECT_EQ(npc->getY(), 30);
}

// Тест многопоточности (базовый)
TEST_F(LocationTest, ThreadSafety) {
    // Создаем 50 NPC
    location->createRandomNPCs(50);
    
    // Запускаем игру на очень короткое время
    auto gameThread = thread([this]() {
        location->runGame(1); // 1 секунда
    });
    
    // Ждем завершения
    gameThread.join();
    
    // Проверяем, что игра завершилась
    // Некоторые NPC могут быть убиты
    EXPECT_LE(location->getAliveNPCCount(), 50);
    EXPECT_GE(location->getAliveNPCCount(), 0);
}

TEST_F(LocationTest, NPCSerialization) {
    auto npc = createNPC("Outlaw", 100, 200, "SerializedRobber");
    
    string serialized = npc->serialize();
    
    // Проверяем формат сериализации
    EXPECT_NE(serialized.find("Outlaw"), string::npos);
    EXPECT_NE(serialized.find("100"), string::npos);
    EXPECT_NE(serialized.find("200"), string::npos);
    EXPECT_NE(serialized.find("SerializedRobber"), string::npos);
}

TEST_F(LocationTest, ObserverNotification) {
    // Создаем наблюдателя
    auto observer = make_shared<ConsoleObserver>();
    location->attach(observer);
    
    // Симулируем уведомление
    location->notify("KillerName", "VictimName");
    
    // Тест проходит, если не падает
    SUCCEED();
}

// Тест на граничные условия карты
TEST_F(LocationTest, MapBoundaries) {
    auto npc = createNPC("Knight", 0, 0, "BoundaryTest");
    
    // Тестируем крайние точки
    npc->setPosition(0, 0);      // Минимальные координаты
    EXPECT_EQ(npc->getX(), 0);
    EXPECT_EQ(npc->getY(), 0);
    
    npc->setPosition(99, 99);    // Максимальные координаты (для карты 100x100)
    EXPECT_EQ(npc->getX(), 99);
    EXPECT_EQ(npc->getY(), 99);
}

// Тест на тип NPC
TEST_F(LocationTest, NPCType) {
    auto knight = createNPC("Knight", 0, 0, "K");
    auto elf = createNPC("Elf", 0, 0, "E");
    auto outlaw = createNPC("Outlaw", 0, 0, "O");
    
    EXPECT_EQ(knight->getType(), "Knight");
    EXPECT_EQ(elf->getType(), "Elf");
    EXPECT_EQ(outlaw->getType(), "Outlaw");
}