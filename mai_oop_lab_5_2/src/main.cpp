#include <iostream>
#include "../include/my_stack.hpp"
#include "../include/person.hpp"

void demonstrate_int_stack() {
    std::cout << "=== Демонстрация MyStack с int ===" << std::endl;
    
    CustomMemoryResource resource;
    MyStack<int> stack(&resource);
    
    // Добавляем элементы
    for (int i = 1; i <= 5; ++i) {
        stack.push(i * 10);
        std::cout << "Добавлен: " << i * 10 << std::endl;
    }
    
    std::cout << "Размер стека: " << stack.size() << std::endl;
    std::cout << "Верхний элемент: " << stack.top() << std::endl;
    
    // Итерация по стеку
    std::cout << "Содержимое стека (через итераторы): ";
    for (auto it = stack.begin(); it != stack.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // Range-based for loop
    std::cout << "Содержимое стека (range-based for): ";
    for (const auto& value : stack) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    
    // Удаляем элементы
    while (!stack.empty()) {
        std::cout << "Удалён: " << stack.top() << std::endl;
        stack.pop();
    }
}

void demonstrate_person_stack() {
    std::cout << "\n=== Демонстрация MyStack с Person ===" << std::endl;
    
    CustomMemoryResource resource;
    MyStack<Person> stack(&resource);
    
    // Добавляем Person объекты
    stack.push(Person("Alice", 25, 50000.0));
    stack.push(Person("Bob", 30, 60000.0));
    stack.push(Person("Charlie", 35, 70000.0));
    
    std::cout << "Размер стека: " << stack.size() << std::endl;
    
    // Доступ через operator->
    std::cout << "\nДоступ через operator->:" << std::endl;
    auto it = stack.begin();
    std::cout << "Имя: " << it->name << std::endl;
    std::cout << "Возраст: " << it->age << std::endl;
    it->celebrate_birthday();
    it->salary += 1000.0;
    
    // Доступ через operator*
    std::cout << "\nДоступ через operator*:" << std::endl;
    std::cout << "Имя: " << (*it).name << std::endl;
    (*it).print();
    
    // Итерация по всем элементам
    std::cout << "\nВсе люди в стеке:" << std::endl;
    for (const auto& person : stack) {
        person.print();
    }
    
    // Использование итераторов вручную
    std::cout << "\nРучная итерация:" << std::endl;
    for (auto it = stack.begin(); it != stack.end(); ++it) {
        std::cout << "Элемент: ";
        it->print();
    }
}

void demonstrate_memory_reuse() {
    std::cout << "\n=== Демонстрация повторного использования памяти ===" << std::endl;
    
    CustomMemoryResource resource;
    MyStack<int> stack(&resource);
    
    // Добавляем и удаляем элементы несколько раз
    for (int cycle = 0; cycle < 3; ++cycle) {
        std::cout << "Цикл " << cycle + 1 << ":" << std::endl;
        
        for (int i = 1; i <= 3; ++i) {
            stack.push(cycle * 10 + i);
            std::cout << "  Добавлен: " << (cycle * 10 + i) << std::endl;
        }
        
        while (!stack.empty()) {
            std::cout << "  Удалён: " << stack.top() << std::endl;
            stack.pop();
        }
    }
}

int main() {
    try {
        demonstrate_int_stack();
        demonstrate_person_stack();
        demonstrate_memory_reuse();
        
        std::cout << "\n=== Все тесты пройдены успешно! ===" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}