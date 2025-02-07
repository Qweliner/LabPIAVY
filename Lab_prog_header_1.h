// Lab_prog_header_1.h
#pragma once
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <consoleapi2.h>
#include <ctype.h>
#include <sys/stat.h>
#include <locale>
#include <string>
#include <algorithm> // Для replace

// Константы, определяющие максимальную длину строки и количество столбцов
const int MAX_LENGTH = 1000;
const int COL_COUNT = 3;

// Глобальные переменные (лучше избегать, но в данном случае для упрощения)
extern char folder_way[256]; // Путь к папке (объявлен как extern, определение в .cpp)
extern char txt_name[256];   // Имя файла (объявлен как extern, определение в .cpp)
extern char text[COL_COUNT][MAX_LENGTH]; // Буфер для ввода текста (объявлен как extern, определение в .cpp)
extern char full_name[512]; // Полный путь к файлу (объявлен как extern, определение в .cpp)
extern const char* file_extension;  // Расширение файла (объявлен как extern, определение в .cpp)
extern const char* ocfe;          // Префикс для файлов исходящей корреспонденции (объявлен как extern, определение в .cpp)
extern const char* oa;          // Префикс для файлов адресов организаций (объявлен как extern, определение в .cpp)
extern char menu_out;         // Выбор пользователя в главном меню (объявлен как extern, определение в .cpp)
extern char menu_out_case;     // Выбор пользователя в подменю (объявлен как extern, определение в .cpp)
extern char sel;            // Выбор пользователя режима записи (объявлен как extern, определение в .cpp)

// Объявления функций
void tabul(int x); // Функция для табуляции
bool is_leap(int year); // Функция для проверки високосного года
bool isValidDate(const char* dateStr); // Функция для проверки корректности даты
bool isValidFileName(const std::string& fileName); // Функция для проверки корректности имени файла
void instruction(); // Функция для отображения инструкции
void outgoing_correspondence(); // Функция для работы с исходящей корреспонденцией
void organization_addresses(); // Функция для работы с адресами организаций
void menu_choises(); // Функция для подменю выбора типа данных
void program_way(); // Функция для выбора пути к папке
void menu(); // Главная функция меню