// Lab_prog_header_1.h
#pragma once

#include <stdio.h>   // ����-�����
#include <stdlib.h>  // ����������� ����������
#include <string.h>  // �������� �� ��������
#include <conio.h>   // ���������� ����-�����
#include <ctype.h>   // ������� ��� ������ � ���������
#include <sys/stat.h>    // stat
#include <direct.h>  // _mkdir � _getcwd
#include <Windows.h> // SetConsoleCP, SetConsoleOutputCP, ����� ������
#include <io.h>      // _access


// ���������� ����������
extern char folder_way[256];
extern const char* file_extension;
extern const char* ocfe;  // ������� ������ ��������� ���������������
extern const char* oa;   // ������� ������ ������� �����������

// ���������� �������
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


// ������ ��� ����������� ����������� �����
#define SAFE_STRCPY(dest, src, size) do { strncpy(dest, src, size - 1); dest[size - 1] = '\0'; } while(0)