#include "gtest/gtest.h"
#include "location/Location.h"
#include "factory/NPCFactory.h"
#include "combat/CombatVisitor.h"
#include "observer/Observer.h"

class MockObserver : public Observer {
public:
    int updateCount = 0;
    std::string lastKiller = "";
    std::string lastVictim = "";

    void update(const std::string& killerName, const std::string& victimName) override {
        updateCount++;
        lastKiller = killerName;
        lastVictim = victimName;
    }
};

class BalagurTest : public ::testing::Test {
protected:
    std::shared_ptr<NPCFactory> factory;
    std::shared_ptr<MockObserver> mockObserver;
    Location* location;

    void SetUp() override {
        factory = std::make_shared<BalagurFateNPCFactory>();
        location = new Location(factory); 
        mockObserver = std::make_shared<MockObserver>();
        location->attach(mockObserver);
    }

    void TearDown() override {
        delete location;
    }
};

// Проверка фабрики
TEST_F(BalagurTest, FactoryCreatesCorrectTypes) {
    auto outlaw = factory->createNPC("Outlaw", 10, 10, "Stark");
    EXPECT_EQ(outlaw->getType(), "Outlaw");

    auto knight = factory->createNPC("Knight", 20, 20, "Arthur");
    EXPECT_EQ(knight->getType(), "Knight");

    auto elf = factory->createNPC("Elf", 30, 30, "Legolas");
    EXPECT_EQ(elf->getType(), "Elf");
}

// Тест: Разбойник убивает Эльфа
TEST_F(BalagurTest, OutlawKillsElf) {
    location->addNPC("Outlaw", 10, 10, "Robber");
    location->addNPC("Elf", 11, 11, "YoungElf");
    
    location->startCombat(5.0); // В радиусе
    
    EXPECT_EQ(mockObserver->updateCount, 1);
    EXPECT_EQ(mockObserver->lastKiller, "Robber");
    EXPECT_EQ(mockObserver->lastVictim, "YoungElf");
}

// Тест: Эльф убивает Рыцаря
TEST_F(BalagurTest, ElfKillsKnight) {
    location->addNPC("Elf", 100, 100, "Archer");
    location->addNPC("Knight", 101, 101, "SirLancelot");
    
    location->startCombat(5.0);
    
    EXPECT_EQ(mockObserver->updateCount, 1);
    EXPECT_EQ(mockObserver->lastKiller, "Archer");
    EXPECT_EQ(mockObserver->lastVictim, "SirLancelot");
}

// Тест: Рыцарь убивает Разбойника
TEST_F(BalagurTest, KnightKillsOutlaw) {
    location->addNPC("Knight", 50, 50, "Guardian");
    location->addNPC("Outlaw", 51, 51, "Thief");
    
    location->startCombat(5.0);
    
    EXPECT_EQ(mockObserver->updateCount, 1);
    EXPECT_EQ(mockObserver->lastKiller, "Guardian");
    EXPECT_EQ(mockObserver->lastVictim, "Thief");
}

// Тест: Разбойник НЕ убивает Рыцаря (Рыцарь должен его убить в ответном ходе)
TEST_F(BalagurTest, OutlawDoesNotKillKnight) {
    location->addNPC("Outlaw", 10, 10, "WeakThief");
    location->addNPC("Knight", 10, 10, "StrongKnight");
    
    location->startCombat(1.0);
    
    // В результате цикла боя Knight убьет Outlaw
    EXPECT_EQ(mockObserver->lastKiller, "StrongKnight");
    EXPECT_EQ(mockObserver->lastVictim, "WeakThief");
}

// Проверка дистанции
TEST_F(BalagurTest, CombatRangeCheck) {
    location->addNPC("Outlaw", 0, 0, "Robber");
    location->addNPC("Elf", 400, 400, "FarElf");
    
    location->startCombat(10.0); // Слишком далеко
    
    EXPECT_EQ(mockObserver->updateCount, 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}