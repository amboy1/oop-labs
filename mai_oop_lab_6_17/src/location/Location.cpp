#include "location/Location.h"
#include <iostream>

void Location::addNPC(const std::string& type, int x, int y, const std::string& name) {
    if (x < 0 || x > 500 || y < 0 || y > 500) {
        throw std::out_of_range("Координаты должны быть в диапазоне [0, 500].");
    }

    try {
        auto newNpc = factory->createNPC(type, x, y, name);
        npcs.push_back(newNpc);
        std::cout << "Добавлен NPC: ";
        newNpc->print();
        std::cout << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Ошибка при создании NPC: " << e.what() << std::endl;
    }
}

void Location::save() {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для сохранения: " + filename);
    }

    for (const auto& npc : npcs) {
        file << npc->serialize() << "\n";
    }
    std::cout << "Локация успешно сохранена в " << filename << std::endl;
}

void Location::load() {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Файл сохранения " << filename << " не найден. Начинаем с пустой локации." << std::endl;
        return;
    }

    npcs.clear();
    std::string line;
    int count = 0;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        try {
            npcs.push_back(factory->createNPC(line));
            count++;
        } catch (const std::exception& e) {
            std::cerr << "Ошибка загрузки строки: " << line << ". " << e.what() << std::endl;
        }
    }
    std::cout << "Загружено " << count << " NPC из " << filename << std::endl;
}

void Location::print() const {
    if (npcs.empty()) {
        std::cout << "Локация пуста." << std::endl;
        return;
    }
    std::cout << "--- Перечень NPC на локации (" << npcs.size() << " объектов) ---" << std::endl;
    int i = 1;
    for (const auto& npc : npcs) {
        std::cout << i++ << ". ";
        npc->print();
        std::cout << std::endl;
    }
    std::cout << "--------------------------------------------------------" << std::endl;
}

void Location::startCombat(double range) {
    if (npcs.size() < 2) {
        std::cout << "Слишком мало NPC для боя. Нужно минимум 2." << std::endl;
        return;
    }
    std::cout << "\nНАЧАЛО БОЯ (Дальность: " << range << "м)" << std::endl;

    CombatEngine visitor(range, this);

    std::vector<std::shared_ptr<NPC>> toRemove;

    for (size_t i = 0; i < npcs.size(); ++i) {
        for (size_t j = i + 1; j < npcs.size(); ++j) {
            bool i_is_removed = std::find(toRemove.begin(), toRemove.end(), npcs[i]) != toRemove.end();
            bool j_is_removed = std::find(toRemove.begin(), toRemove.end(), npcs[j]) != toRemove.end();

            if (i_is_removed || j_is_removed) continue;

            bool i_wins = npcs[i]->accept(&visitor, npcs[j].get());

            if (i_wins) {
                if (!j_is_removed) toRemove.push_back(npcs[j]);
            } else {
                bool j_wins = npcs[j]->accept(&visitor, npcs[i].get());

                if (j_wins) {
                    if (!i_is_removed) toRemove.push_back(npcs[i]);
                }
            }
        }
    }
    size_t initial_size = npcs.size();
    for (const auto& victim : toRemove) {
        npcs.erase(std::remove(npcs.begin(), npcs.end(), victim), npcs.end());
    }

    std::cout << "--- БОЙ ЗАВЕРШЕН ---" << std::endl;
    std::cout << "Погибло NPC: " << (initial_size - npcs.size()) << std::endl;
    print();
}