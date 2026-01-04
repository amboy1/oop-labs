#include "factory/NPCFactory.h"
#include <sstream>
#include <stdexcept>

BalagurFateNPCFactory::BalagurFateNPCFactory() {
    creators["Outlaw"] = [](int x, int y, const std::string& name) { return std::make_shared<Outlaw>(x, y, name); };
    creators["Knight"] = [](int x, int y, const std::string& name) { return std::make_shared<Knight>(x, y, name); };
    creators["Elf"]    = [](int x, int y, const std::string& name) { return std::make_shared<Elf>(x, y, name); };
}

std::shared_ptr<NPC> BalagurFateNPCFactory::createNPC(const std::string& type, int x, int y, const std::string& name) {
    auto it = creators.find(type);
    if (it != creators.end()) {
        return it->second(x, y, name);
    }
    throw std::invalid_argument("Неизвестный тип NPC: " + type);
}

std::shared_ptr<NPC> BalagurFateNPCFactory::createNPC(const std::string& serializedData) {
    std::stringstream ss(serializedData);
    std::string type, name;
    int x, y;

    if (!(ss >> type >> x >> y >> name)) {
        throw std::runtime_error("Неверный формат данных для NPC: " + serializedData);
    }
    return createNPC(type, x, y, name);
}