#ifndef PERSON_HPP
#define PERSON_HPP

#include <iostream>
#include <string>

struct Person {
    std::string name;
    int age;
    double salary;
    
    Person(std::string n = "", int a = 0, double s = 0.0) 
        : name(n), age(a), salary(s) {}
    
    void print() const {
        std::cout << name << " (age: " << age << ", salary: " << salary << ")" << std::endl;
    }
    
    void celebrate_birthday() {
        ++age;
        std::cout << name << " is now " << age << " years old!" << std::endl;
    }
    
    // Операторы сравнения для тестов
    bool operator==(const Person& other) const {
        return name == other.name && age == other.age && salary == other.salary;
    }
    
    bool operator!=(const Person& other) const {
        return !(*this == other);
    }
};

#endif