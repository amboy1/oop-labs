#pragma once
#include "../location/NPC.h"
#include <memory>
#include <map>
#include <functional>

class NPCFactory {
public:
    virtual ~NPCFactory() = default;

    virtual std::shared_ptr<NPC> createNPC(const std::string& type, int x, int y, const std::string& name) = 0;

    virtual std::shared_ptr<NPC> createNPC(const std::string& serializedData) = 0;
};

class BalagurFateNPCFactory : public NPCFactory {
private:
    std::map<std::string, std::function<std::shared_ptr<NPC>(int, int, const std::string&)>> creators;

public:
    BalagurFateNPCFactory();

    std::shared_ptr<NPC> createNPC(const std::string& type, int x, int y, const std::string& name) override;

    std::shared_ptr<NPC> createNPC(const std::string& serializedData) override;
};