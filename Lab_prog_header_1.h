//Lab_prog_header_1.h
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
#include <algorithm>
#include <filesystem>

// Глобальные переменные
extern char folder_way[256];
extern const char* file_extension;
extern const char* ocfe;
extern const char* oa;

// Объявления функций
bool is_leap(int year);
bool isValidDate(const char* dateStr);
bool isValidFileName(const std::string& fileName);
void instruction();
void outgoing_correspondence();
void organization_addresses();
void menu_choises();
void program_way();
void menu();
std::string getLineWithEsc(const std::string& instruction);
std::string normalizePath(const std::string& path);
void openFileForAppend(FILE*& file, const char* full_name, const char* headers, std::string& mode);