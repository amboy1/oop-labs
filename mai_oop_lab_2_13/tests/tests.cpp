#include <gtest/gtest.h>
#include "../src/binary.h" // Update the path to the correct location of binary.h

TEST(BinaryTest, AdditionSameSize) {
	Binary b1({1, 0, 1}); // 5 в двоичной системе
	Binary b2({1, 1, 0}); // 6 в двоичной системе
	Binary result = b1 + b2; // Ожидаем 11 (1011 в двоичной системе)

	EXPECT_EQ(result, Binary({1, 0, 1, 1}));
}

TEST(BinaryTest, AdditionDifferentSize) {
	Binary b1({1, 0, 1}); // 5 в двоичной системе
	Binary b2({1, 1});    // 3 в двоичной системе
	Binary result = b1 + b2; // Ожидаем 8 (1000 в двоичной системе)

	EXPECT_EQ(result, Binary({1, 0, 0, 0}));
}

TEST(BinaryTest, AdditionWithCarry) {
	Binary b1({1, 1, 1}); // 7 в двоичной системе
	Binary b2({1});       // 1 в двоичной системе
	Binary result = b1 + b2; // Ожидаем 8 (1000 в двоичной системе)

	EXPECT_EQ(result, Binary({1, 0, 0, 0}));
}


TEST(BinaryTest, SubtractionSameSize) {
    Binary b1({1, 0, 1}); // 5 в двоичной системе
    Binary b2({1, 0, 0}); // 4 в двоичной системе
    Binary result = b1 - b2; // Ожидаем 1 (001 в двоичной системе)

    EXPECT_EQ(result, Binary({1}));
}

TEST(BinaryTest, SubtractionDifferentSize) {
    Binary b1({1, 0, 1}); // 5 в двоичной системе
    Binary b2({1});       // 1 в двоичной системе
    Binary result = b1 - b2; // Ожидаем 4 (100 в двоичной системе)

    EXPECT_EQ(result, Binary({1, 0, 0}));
}

TEST(BinaryTest, SubtractionWithBorrow) {
    Binary b1({1, 0, 0, 0}); // 8
    Binary b2({1});          // 1
    Binary result = b1 - b2; // 7
    
    EXPECT_EQ(result, Binary({1, 1, 1})); // Правильный результат без ведущих нулей
}

TEST(BinaryTest, SubtractionNegative) {
    Binary b1({1, 0, 0}); // 4 в двоичной системе
    Binary b2({1, 0, 1}); // 5 в двоичной системе

    EXPECT_THROW(b1 - b2, std::invalid_argument); // Ожидаем исключение
}


TEST(BinaryTest, EqualityOperator) {
    Binary b1({1, 0, 1});
    Binary b2({1, 0, 1});
    Binary b3({1, 1, 0});

    EXPECT_TRUE(b1 == b2); // Ожидаем равенство
    EXPECT_FALSE(b1 == b3); // Ожидаем неравенство
}

TEST(BinaryTest, InequalityOperator) {
    Binary b1({1, 0, 1});
    Binary b2({1, 1, 0});

    EXPECT_TRUE(b1 != b2); // Ожидаем неравенство
    EXPECT_FALSE(b1 != b1); // Ожидаем равенство
}

TEST(BinaryTest, LessThanOperator) {
    Binary b1({1, 0, 1}); // 5
    Binary b2({1, 1, 0}); // 6

    EXPECT_TRUE(b1 < b2); // 5 < 6
    EXPECT_FALSE(b2 < b1); // 6 < 5 — ложь
}

TEST(BinaryTest, GreaterThanOperator) {
    Binary b1({1, 0, 1}); // 5
    Binary b2({1, 1, 0}); // 6

    EXPECT_TRUE(b2 > b1); // 6 > 5
    EXPECT_FALSE(b1 > b2); // 5 > 6 — ложь
}


TEST(BinaryTest, InitializationFromList) {
    Binary b({1, 0, 1});

    EXPECT_EQ(b, Binary({1, 0, 1})); // Проверяем корректность инициализации
}

TEST(BinaryTest, InitializationFromEmptyList) {
    Binary b({});

    EXPECT_EQ(b, Binary({})); // Пустой объект должен быть корректно инициализирован
}


TEST(BinaryTest, InvalidDataInitialization) {
    EXPECT_THROW(Binary({2, 0, 1}), std::invalid_argument); // Должно выбросить исключение
}


TEST(BinaryTest, ZeroBinary) {
    // Разные представления нуля считаются разными объектами
    Binary b1({0, 0, 0});
    Binary b2({0});
    
    EXPECT_FALSE(b1 == b2); // Они разные по размеру
    EXPECT_TRUE(b1 - b1 == b2); // Но математически равны
    EXPECT_TRUE(b2 - b2 == b2); // 0 - 0 = 0
}

TEST(BinaryTest, ZeroAddition) {
    Binary b1({0, 0, 0});
    Binary b2({1, 0, 1});

    EXPECT_EQ(b1 + b2, b2); // 0 + 5 = 5
}

TEST(BinaryTest, ZeroSubtraction) {
    Binary b1({1, 0, 1});
    Binary b2({0, 0, 0});

    EXPECT_EQ(b1 - b2, b1); // 5 - 0 = 5
}
