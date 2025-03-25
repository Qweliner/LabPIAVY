// Lab_prog_header_1.h
#pragma once

#include <stdio.h>   // Ввод-вывод
#include <stdlib.h>  // Стандартная библиотека
#include <string.h>  // Операции со строками
#include <conio.h>   // Консольный ввод-вывод
#include <ctype.h>   // Функции для работы с символами
#include <sys/stat.h>    // stat
#include <direct.h>  // _mkdir и _getcwd
#include <Windows.h> // SetConsoleCP, SetConsoleOutputCP, буфер обмена
#include <io.h>      // _access


// Глобальные переменные
extern char folder_way[256];
extern const char* file_extension;
extern const char* ocfe;  // Префикс файлов исходящей корреспонденции
extern const char* oa;   // Префикс файлов адресов организаций

// Объявления функций
bool is_leap(int year);
bool isValidDate(const char* dateStr);
bool isValidFileName(const char* fileName);
void instruction();
void outgoing_correspondence();
void organization_addresses();
void menu_choises();
void program_way();
void menu();
void getLineWithEsc(const char* instruction, char* buffer, int buffer_size);
void normalizePath(const char* path, char* normalized_path, size_t normalized_path_size);
void openFileForAppend(FILE*& file, const char* full_name, const char* headers, char* mode, size_t mode_size);


// Макрос для безопасного копирования строк
#define SAFE_STRCPY(dest, src, size) do { strncpy(dest, src, size - 1); dest[size - 1] = '\0'; } while(0)