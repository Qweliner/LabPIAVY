// Lab_prog_1.cpp
#include "Lab_prog_header_1.h"

int main()
{
    // Устанавливаем кодировку консоли для корректного отображения кириллицы
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // Устанавливаем изначальный размер окна консольного приложения
    system("mode con cols=120 lines=30");

    // Вызываем главную функцию меню
    menu();

    return 0;
}