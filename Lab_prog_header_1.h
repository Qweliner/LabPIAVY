#pragma once

#include <stdio.h>   // ����-�����: printf, fprintf, fopen, fclose, fgets, sscanf, snprintf
#include <stdlib.h>  // ����������� ����������: system, malloc, free, atoi (���� �����������)
#include <string.h>  // �������� �� ��������: strcpy, strncpy, strcmp, strlen, strncat, strtok, strrchr
#include <conio.h>   // ���������� ����-�����: _getch, _kbhit
#include <ctype.h>   // ������� ��� ������ � ���������: isalpha, tolower, isdigit
#include <sys/stat.h>    // ��� stat: ����������� ���� �����/�����
#include <direct.h>  // ��� _mkdir (Windows) � _getcwd: �������� ����� � ��������� �������� ����
#include <Windows.h> // ��� SetConsoleCP, SetConsoleOutputCP, ������ ������: ��������� ��������� �������
#include <io.h>      // ��� _access: �������� ������������� �����


// ���������� ����������
// ������ 256 ������ ��� �����������, �� �� ���������� �������� ��� ����������� ����� � ������ � ������ ������.
// ��� ���������� ����� ������������ ������������ ������� ���� � ��������� ������.
// �����, ������ ������ ��������� ���������� MAX_PATH (260 ��������), ������������ � Windows API.
extern char folder_way[256];
extern const char* file_extension;
extern const char* ocfe;  // ������� ��� ������ ��������� ���������������
extern const char* oa;   // ������� ��� ������ ������� �����������

// ���������� �������
bool is_leap(int year);
bool isValidDate(const char* dateStr);
bool isValidFileNameChar(char c); // �������� ������� ��� ����� �����
bool isValidPathChar(char c); // �������� ������� ��� ����
void instruction();
void outgoing_correspondence();
void organization_addresses();
void menu_choises();
void program_way();
void menu();
void getLineWithRestrictedChars(const char* instruction, char* buffer, int buffer_size, bool (*isValidCharFunc)(char)); // ������� ����� � ��������� ��������
void normalizePath(const char* path, char* normalized_path, size_t normalized_path_size);
void openFileForAppend(FILE*& file, const char* full_name, const char* headers); // ������ mode � mode_size, ��� �� ������������


// ������ ��� ����������� ����������� �����.  �� ���������� strncpy, �� �����������,
// ��� ������-��������� ����� ��������� ����-������������ ('\0'), ���� ���� ��������
// ������ ������� ������� � ����������.  ��� ������������� ������������ ������ �
// ��������� � ��� ������.  `do { ... } while(0)` ������������ ��� ����, �����
// ������ ����� ���� ������������ � ����� �����, ��� �������� ��������, ��������,
// � ���� `if` ��� �������� ������.
#define SAFE_STRCPY(dest, src, size) do { strncpy(dest, src, size - 1); dest[size - 1] = '\0'; } while(0)