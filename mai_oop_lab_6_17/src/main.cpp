#include "location/Location.h"
#include "factory/NPCFactory.h"
#include "observer/Observer.h"
#include <limits>
#include <memory>

using namespace std;

void showMenu() {
    cout << "\n--- Редактор Подземелья Balagur Fate 3 (Вариант 17) ---" << endl;
    cout << "1. Добавить NPC (Outlaw, Knight, Elf)" << endl;
    cout << "2. Сохранить локацию" << endl;
    cout << "3. Загрузить локацию" << endl;
    cout << "4. Печать перечня NPC" << endl;
    cout << "5. Запустить боевой режим" << endl;
    cout << "0. Выход" << endl;
    cout << "Выберите действие: ";
}

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    shared_ptr<NPCFactory> factory = make_shared<BalagurFateNPCFactory>();
    Location location(factory);

    shared_ptr<Observer> consoleObserver = make_shared<ConsoleObserver>();
    shared_ptr<Observer> fileObserver = make_shared<FileObserver>();

    location.attach(consoleObserver);
    location.attach(fileObserver);

    int choice;
    location.load();

    do {
        showMenu();
        if (!(cin >> choice)) {
            clearInput();
            choice = -1;
            continue;
        }

        try {
            switch (choice) {
                case 1: {
                    string type, name;
                    int x, y;
                    cout << "Введите тип (Outlaw, Knight, Elf): ";
                    cin >> type;
                    cout << "Введите имя: ";
                    cin >> name;
                    cout << "Введите X (0-500): ";
                    cin >> x;
                    cout << "Введите Y (0-500): ";
                    cin >> y;
                    location.addNPC(type, x, y, name);
                    break;
                }
                case 2:
                    location.save();
                    break;
                case 3:
                    location.load();
                    break;
                case 4:
                    location.print();
                    break;
                case 5: {
                    double range;
                    cout << "Введите дальность боя (метры): ";
                    if (!(cin >> range) || range < 0) {
                         cout << "Неверный ввод. Дальность должна быть положительным числом." << endl;
                         clearInput();
                         break;
                    }
                    location.startCombat(range);
                    break;
                }
                case 0:
                    cout << "Выход из редактора. До свидания!" << endl;
                    break;
                default:
                    cout << "Неверный выбор. Попробуйте снова." << endl;
                    break;
            }
        } catch (const exception& e) {
            cerr << "Ошибка: " << e.what() << endl;
        }

    } while (choice != 0);

    return 0;
}