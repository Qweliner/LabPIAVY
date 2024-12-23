// Instruction.h
#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>

void instruction() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    system("cls");
    std::ifstream file("Instruction.txt");
    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            std::cout << line << std::endl;
        }
        file.close();
    }
    else
    {
        std::cout << "Не удалось открыть файл instruction.txt" << std::endl;
    }



    printf("\n> ");
    system("PAUSE>nul");
}