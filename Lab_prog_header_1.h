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
#include <filesystem> // Добавили

// Глобальные переменные (оставлены только необходимые, аналогичные второй программе)
extern char folder_way[256];       // Путь к папке
extern const char* file_extension; // Расширение файла
extern const char* ocfe;           // Префикс для файлов исходящей корреспонденции
extern const char* oa;             // Префикс для файлов адресов организаций

// Объявления функций
bool is_leap(int year);                                 // Функция для проверки високосного года
bool isValidDate(const char* dateStr);                  // Функция для проверки корректности даты
bool isValidFileName(const std::string& fileName);      // Функция для проверки корректности имени файла
void instruction();                                     // Функция для отображения инструкции
void outgoing_correspondence();                         // Функция для работы с исходящей корреспонденцией
void organization_addresses();                           // Функция для работы с адресами организаций
void menu_choises();                                    // Функция для подменю выбора типа данных
void program_way();                                     // Функция для выбора пути к папке
void menu();                                            // Главная функция меню
std::string getLineWithEsc(const std::string& instruction); //Функция ввода с обработкой Esc
std::string normalizePath(const std::string& path);