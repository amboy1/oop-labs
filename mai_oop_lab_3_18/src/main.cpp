#include <iostream>
#include "rectangle.h"
#include "trapezoid.h"
#include "square.h"
#include "figure_array.h"

int main() {
    FigureArray figures;

    int n;
    std::cout << "Введите количество фигур: ";
    std::cin >> n;

    for (int i = 0; i < n; ++i) {
        std::cout << "\nФигура №" << (i + 1) << '\n';
        std::cout << "Выберите тип (1 - прямоугольник, 2 - трапеция, 3 - квадрат): ";

        int type;
        std::cin >> type;

        switch (type) {
            case 1: {
                double w, h;
                std::cout << "Введите ширину и высоту прямоугольника: ";
                std::cin >> w >> h;
                figures.add(new Rectangle(w, h, "Прямоугольник"));
                break;
            }

            case 2: {
                double a, b, h;
                std::cout << "Введите нижнее основание, верхнее основание и высоту трапеции: ";
                std::cin >> a >> b >> h;
                figures.add(new Trapezoid(a, b, h, "Трапеция"));
                break;
            }

            case 3: {
                double side;
                std::cout << "Введите длину стороны квадрата: ";
                std::cin >> side;
                figures.add(new Square(side, "Квадрат"));
                break;
            }

            default:
                std::cout << "Неизвестный тип фигуры, попробуйте снова.\n";
                --i; // Повторяем ввод
                break;
        }
    }

    std::cout << "\nСписок всех фигур:\n";
    figures.printAll();

    std::cout << "\nСуммарная площадь: " << figures.totalArea() << "\n";

    std::cout << "\nВведите номер фигуры для удаления: ";
    int index;
    std::cin >> index;

    try {
        figures.remove(index);
        std::cout << "Фигура удалена.\n";
    } catch (const std::exception& e) {
        std::cout << "Ошибка при удалении: " << e.what() << "\n";
    }

    std::cout << "\nОставшиеся фигуры:\n";
    figures.printAll();

    std::cout << "\nИтоговая площадь после удаления: " << figures.totalArea() << "\n";

    return 0;
}
