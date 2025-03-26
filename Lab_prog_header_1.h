#pragma once

#include <stdio.h>   // Ввод-вывод: printf, fprintf, fopen, fclose, fgets, sscanf, snprintf
#include <stdlib.h>  // Стандартная библиотека: system, malloc, free, atoi (если потребуется)
#include <string.h>  // Операции со строками: strcpy, strncpy, strcmp, strlen, strncat, strtok, strrchr
#include <conio.h>   // Консольный ввод-вывод: _getch, _kbhit
#include <ctype.h>   // Функции для работы с символами: isalpha, tolower, isdigit
#include <sys/stat.h>    // Для stat: определение типа файла/папки
#include <direct.h>  // Для _mkdir (Windows) и _getcwd: создание папок и получение текущего пути
#include <Windows.h> // Для SetConsoleCP, SetConsoleOutputCP, буфера обмена: установка кодировки консоли
#include <io.h>      // Для _access: проверка существования файла


// Глобальные переменные
// Размер 256 выбран как достаточное, но не чрезмерное значение для большинства путей к файлам и именам файлов.
// Это компромисс между возможностью обрабатывать длинные пути и экономией памяти.
// Также, размер буфера ограничен константой MAX_PATH (260 символов), определенной в Windows API.
extern char folder_way[256];
extern const char* file_extension;
extern const char* ocfe;  // Префикс для файлов исходящей корреспонденции
extern const char* oa;   // Префикс для файлов адресов организаций

// Объявления функций
bool is_leap(int year);
bool isValidDate(const char* dateStr);
bool isValidFileNameChar(char c); // Проверка символа для имени файла
bool isValidPathChar(char c); // Проверка символа для пути
void instruction();
void outgoing_correspondence();
void organization_addresses();
void menu_choises();
void program_way();
void menu();
void getLineWithRestrictedChars(const char* instruction, char* buffer, int buffer_size, bool (*isValidCharFunc)(char)); // Функция ввода с проверкой символов
void normalizePath(const char* path, char* normalized_path, size_t normalized_path_size);
void openFileForAppend(FILE*& file, const char* full_name, const char* headers); // Убраны mode и mode_size, они не используются


// Макрос для безопасного копирования строк.  Он аналогичен strncpy, но гарантирует,
// что строка-результат будет завершена нуль-терминатором ('\0'), даже если исходная
// строка слишком длинная и обрезается.  Это предотвращает переполнение буфера и
// связанные с ним ошибки.  `do { ... } while(0)` используется для того, чтобы
// макрос можно было использовать в любом месте, где допустим оператор, например,
// в теле `if` без фигурных скобок.
#define SAFE_STRCPY(dest, src, size) do { strncpy(dest, src, size - 1); dest[size - 1] = '\0'; } while(0)