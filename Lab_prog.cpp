// Lab_prog_1.cpp
#include "Lab_prog_header_1.h"

int main()
{
    // Устанавливаем кодировку консоли для корректного отображения кириллицы
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // Вызываем главную функцию меню
    menu();

    return 0;
}