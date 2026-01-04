#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <memory>

class Subject {
private:
    std::list<std::shared_ptr<class Observer>> observers;

public:
    void attach(std::shared_ptr<Observer> observer) {
        observers.push_back(observer);
    }

    void detach(std::shared_ptr<Observer> observer);

    void notify(const std::string& killerName, const std::string& victimName);
};

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(const std::string& killerName, const std::string& victimName) = 0;
};

class ConsoleObserver : public Observer {
public:
    void update(const std::string& killerName, const std::string& victimName) override {
        std::cout << "**БОЙ:** **" << killerName << "** убивает **" << victimName << "**" << std::endl;
    }
};

class FileObserver : public Observer {
private:
    std::string filename = "log.txt";
public:
    void update(const std::string& killerName, const std::string& victimName) override {
        std::ofstream file(filename, std::ios::app);
        if (file.is_open()) {
            file << "Убийство: " << killerName << " -> " << victimName << std::endl;
        }
    }
};