#include "observer/Observer.h"
#include <algorithm>
#include <iostream>

void Subject::detach(std::shared_ptr<Observer> observer) {
    observers.remove(observer);
}

void Subject::notify(const std::string& killerName, const std::string& victimName) {
    for (const auto& observer : observers) {
        observer->update(killerName, victimName);
    }
}