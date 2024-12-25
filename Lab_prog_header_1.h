// Prog_1_header.h
#pragma once
#define _CRT_SECURE_NO_WARNINGS
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


#define MAX_LENGTH 1000
#define COL_COUNT 3

using namespace std;

char folder_way[256];
char txt_name[256];
char text[COL_COUNT][MAX_LENGTH];
char full_name[512];
const char* file_extension = ".txt";
const char* ocfe = "IC_"; // Исходящая корреспонденция
const char* oa = "AO_"; // Адреса организаций
char menu_out;
char menu_out_case;
char sel;

void menu();
void menu_choises();

void tabul(int x) {
    for (int i = x; i != 0; i--) printf("   ");
}

bool is_leap(int year) {
    return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
}

bool isValidDate(const char* dateStr) {
    if (strlen(dateStr) != 10) {
        return false;
    }

    int day, month, year;
    int count = sscanf(dateStr, "%2d.%2d.%4d", &day, &month, &year);

    if (count != 3) {
        return false;
    }
    if (month < 1 || month > 12) {
        return false;
    }
    if (day < 1 || day > 31) {
        return false;
    }
    if (month == 2) {
        int daysInFeb = 28 + (is_leap(year) ? 1 : 0);
        if (day > 28 + (is_leap(year) ? 1 : 0)) {
            return false;
        }
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30) {
            return false;
        }
    }

    return true;
}

bool isValidFileName(const std::string& fileName) {
    // Запрещенные символы в Windows
    const std::string invalidChars = "\\/:*?\"<>|";
    return fileName.find_first_of(invalidChars) == std::string::npos;
}

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

void outgoing_correspondence() {
    do {
        system("cls");
        SetConsoleCP(1251);
        SetConsoleOutputCP(1251);
        std::locale loc("ru_RU.UTF-8");
        for (int i = 4; i != 0; i--) printf("\n");
        tabul(11); printf("ИСХОДЯЩАЯ КОРРЕСПОНДЕНЦИЯ\n\n");
        tabul(11); printf("1)   СОЗДАТЬ НОВЫЙ ФАЙЛ\n");
        tabul(11); printf("2)   ОТКРЫТЬ ФАЙЛ\n");
        tabul(11); printf("Esc) ВЫХОД В ВЫБОР ТИПА ФАЙЛА\n\n");
        tabul(11); printf("_____________________________________________\n\n");
        tabul(11); printf("> ");
        menu_out_case = _getch();
        FILE* file{};
        switch (menu_out_case) {
        case '1': {
            printf("\n");
            tabul(11); printf("ВВЕДИТЕ ИМЯ ФАЙЛА: ");
            string fileNameInput;
            scanf("%255[^\n]", txt_name);
            getchar(); // очищаем буфер
            if (!isValidFileName(txt_name)) {
                tabul(11); printf("Недопустимые символы в имени файла. Пожалуйста, используйте другое имя.\n");
                printf("ДЛЯ ПРОДОЛЖЕНИЯ НАЖМИТЕ ENTER."); system("PAUSE>nul");
                break;
            }


            sprintf(full_name, "%s%s%s%s", folder_way, ocfe, txt_name, file_extension);
            file = fopen(full_name, "w");
            if (file) {
                fclose(file);
                tabul(11); printf("ФАЙЛ УСПЕШНО СОЗДАН. ");
            }
            else {
                tabul(11); printf("ОШИБКА СОЗДАНИЯ. ФАЙЛ НЕ БЫЛ СОЗДАН. ");
            }
            printf("ДЛЯ ПРОДОЛЖЕНИЯ НАЖМИТЕ ENTER."); system("PAUSE>nul");
            break;
        }

        case '2': {
            int i = 0;
            int current_col = 0;
            printf("\n");
            tabul(11); printf("ВВЕДИТЕ ИМЯ ФАЙЛА: "); scanf("%255s", txt_name);
            tabul(11); printf("ВЫБЕРИТЕ РЕЖИМ РАБОТЫ С ФАЙЛОМ: \n");
            tabul(11); printf("1)   ЗАПИСЬ В ФАЙЛ\n");
            tabul(11); printf("2)   ДОБАВИТЬ В ФАЙЛ\n");
            tabul(11); printf("Esc) ВЫХОД В ВЫБОР ФАЙЛА\n\n");
            sprintf(full_name, "%s%s%s%s", folder_way, ocfe, txt_name, file_extension);
            tabul(11); printf("> ");
            sel = _getch();
            if (sel == 27) { break; } // Добавлен выход по Esc
            switch (sel) {
            case '1':
                file = fopen(full_name, "w");
                fprintf(file, "ТИП КОРРЕСПОНДЕНЦИИ | ДАТА | НАЗВАНИЕ ОРГАНИЗАЦИИ\n");
                break;
            case '2':
                file = fopen(full_name, "a");
                break;
            default:
                break; // Добавлен default case для обработки некорректного ввода
            }
            if (sel != '1' && sel != '2') { break; }
            system("cls");
            printf("ФАЙЛ ОТКРЫТ.\n\n");
            printf("Нажмите TAB для перехода к следующему параметру.\n");
            printf("Нажмите Enter для моментального перехода к следующей записи.\n");
            printf("Обратите внимание что при нажатии TAB при отсутсвии ввода программа так же перейдет к следующему параметру!\n");
            while (1) {

                if (current_col == 0) {
                    printf("\nТИП КОРРЕСПОНДЕНЦИИ: ");
                }
                else if (current_col == 1) {
                    printf("\nДАТА (ДД.ММ.ГГГГ): ");
                }
                else if (current_col == 2) {
                    printf("\nНАЗВАНИЕ ОРГАНИЗАЦИИ: ");
                }
                i = 0;
                while (1) {
                    char c = _getch();

                    if (c == 27) {
                        goto exit_outgoing_correspondence;
                    }
                    else if (c == '\t') {
                        text[current_col][i] = '\0';
                        
                        if (current_col == 1 && !isValidDate(text[1]) && text[1][0] != '\0') {
                            printf("\nНеверный формат даты. Пожалуйста, введите дату в формате ДД.ММ.ГГГГ\n");
                            memset(text[current_col], 0, sizeof(text[current_col]));
                            i = 0;
                            continue;
                        }


                        current_col++;
                        if (current_col == COL_COUNT) {
                            for (int j = 0; j < COL_COUNT; ++j) {
                                if (strlen(text[j]) == 0) {
                                    strcpy(text[j], "нет данных");
                                }
                            }
                            fprintf(file, "%s\t%s\t%s\n", text[0], text[1], text[2]);
                            current_col = 0;
                            memset(text, 0, sizeof(text));
                            printf("\n");
                            break;
                        }
                        break;

                    }
                    else if (c == '\r') {
                        text[current_col][i] = '\0';
                        for (int j = 0; j < COL_COUNT; ++j) {
                            if (strlen(text[j]) == 0) {
                                strcpy(text[j], "нет данных");
                            }
                        }

                        if (current_col == 1 && !isValidDate(text[1]))
                        {
                            printf("\nНеверный формат даты. Пожалуйста, введите дату в формате ДД.ММ.ГГГГ\n");
                            memset(text[current_col], 0, sizeof(text[current_col]));
                            i = 0;
                            continue;
                        }
                        fprintf(file, "%s\t%s\t%s\n", text[0], text[1], text[2]);
                        current_col = 0;
                        memset(text, 0, sizeof(text));
                        printf("\n");
                        break;
                    }
                    else if (c == 8) {
                        if (i > 0) {
                            i--;
                            printf("\b \b");
                        }
                    }
                    else {
                        if (current_col == 1) { // Только для колонки с датой
                            if (!isdigit(c) && c != '.' && c != '\t' && c != '\r' && c != 8) {
                                continue;
                            }
                            if (i < MAX_LENGTH - 1) {
                                if (c >= -64 && c <= -1) {
                                    continue;
                                }
                                text[current_col][i++] = c;
                                printf("%c", c);
                            }
                        }
                        else { // Для остальных колонок
                            if (i < MAX_LENGTH - 1) {
                                text[current_col][i++] = c;
                                printf("%c", c);
                            }
                        }
                    }
                }
            }
        exit_outgoing_correspondence:
            if (file) fclose(file);
            outgoing_correspondence();
            return;
            break;
        }
        }
    } while (menu_out_case != 27);
}

void organization_addresses() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::locale loc("ru_RU.UTF-8");


    do {
        system("cls");
        for (int i = 4; i != 0; i--) printf("\n");
        tabul(10); printf("АДРЕСА ОРГАНИЗАЦИИ\n\n");
        tabul(11); printf("1)   СОЗДАТЬ НОВЫЙ ФАЙЛ\n");
        tabul(11); printf("2)   ОТКРЫТЬ ФАЙЛ\n");
        tabul(11); printf("Esc) ВЫХОД В ВЫБОР ТИПА ФАЙЛА\n\n");
        tabul(11); printf("_____________________________________________\n\n");
        tabul(11); printf("> ");
        menu_out_case = _getch();
        FILE* file{};
        switch (menu_out_case) {
        case '1': {
            printf("\n");
            tabul(11); printf("ВВЕДИТЕ ИМЯ ФАЙЛА: ");

            scanf("%255[^\n]", txt_name);
            getchar();
            if (!isValidFileName(txt_name)) {
                tabul(11); printf("Недопустимые символы в имени файла. Пожалуйста, используйте другое имя.\n");
                printf("ДЛЯ ПРОДОЛЖЕНИЯ НАЖМИТЕ ENTER."); system("PAUSE>nul");
                break;
            }


            sprintf(full_name, "%s%s%s%s", folder_way, oa, txt_name, file_extension);
            file = fopen(full_name, "w");
            if (file) {
                fclose(file);
                tabul(11); printf("ФАЙЛ УСПЕШНО СОЗДАН. ");
            }
            else {
                tabul(11); printf("ОШИБКА СОЗДАНИЯ. ФАЙЛ НЕ БЫЛ СОЗДАН. ");
            }
            printf("ДЛЯ ПРОДОЛЖЕНИЯ НАЖМИТЕ ENTER."); system("PAUSE>nul");
            break;
        }

        case '2': {
            int i = 0;
            int current_col = 0;
            printf("\n");
            tabul(11); printf("ВВЕДИТЕ ИМЯ ФАЙЛА: "); scanf("%255s", txt_name);
            tabul(11); printf("ВЫБЕРИТЕ РЕЖИМ РАБОТЫ С ФАЙЛОМ: \n");
            tabul(11); printf("1) ЗАПИСЬ В ФАЙЛ\n");
            tabul(11); printf("2) ДОБАВИТЬ В ФАЙЛ\n");
            tabul(11); printf("Esc)ВЫХОД В ВЫБОР ФАЙЛА\n\n");
            sprintf(full_name, "%s%s%s%s", folder_way, oa, txt_name, file_extension);
            tabul(11); printf("> ");
            sel = _getch();
            if (sel == 27) { break; } // Добавлен выход по Esc
            switch (sel) {
            case '1':
                file = fopen(full_name, "w");
                fprintf(file, "НАЗВАНИЕ ОРГАНИЗАЦИИ | АДРЕС | ФИО ОРГАНИЗАТОРА\n");
                break;
            case '2':
                file = fopen(full_name, "a");
                break;
            default:
                break; // Добавлен default case для обработки некорректного ввода
            }
            if (sel != '1' && sel != '2') { break; }
            system("cls");
            printf("ФАЙЛ ОТКРЫТ.\n\n");
            printf("Нажмите TAB для перехода к следующему параметру.\n");
            printf("Нажмите Enter для моментального перехода к следующей записи.\n");
            printf("Обратите внимание что при нажатии TAB при отсутсвии ввода программа так же перейдет к следующему параметру!\n");
            while (1) {

                if (current_col == 0) {
                    printf("\nНАЗВАНИЕ ОРГАНИЗАЦИИ: ");
                }
                else if (current_col == 1) {
                    printf("\nАДРЕС: ");
                }
                else if (current_col == 2) {
                    printf("\nФИО ОРГАНИЗАТОРА: ");
                }
                i = 0;
                while (1) {
                    char c = _getch();

                    if (c == 27) {
                        goto exit_organization_addresses;
                    }
                    else if (c == '\t') {
                        text[current_col][i] = '\0';
                        current_col++;
                        if (current_col == COL_COUNT) {
                            for (int j = 0; j < COL_COUNT; ++j) {
                                if (strlen(text[j]) == 0) {
                                    strcpy(text[j], "нет данных");
                                }
                            }
                            fprintf(file, "%s\t%s\t%s\n", text[0], text[1], text[2]);
                            current_col = 0;
                            memset(text, 0, sizeof(text));
                            printf("\n");
                            break;
                        }
                        break;

                    }
                    else if (c == '\r') {
                        text[current_col][i] = '\0';
                        for (int j = 0; j < COL_COUNT; ++j) {
                            if (strlen(text[j]) == 0) {
                                strcpy(text[j], "нет данных");
                            }
                        }
                        fprintf(file, "%s\t%s\t%s\n", text[0], text[1], text[2]);
                        current_col = 0;
                        memset(text, 0, sizeof(text));
                        printf("\n");
                        break;
                    }
                    else if (c == 8) {
                        if (i > 0) {
                            i--;
                            printf("\b \b");
                        }
                    }
                    else {

                        if (current_col == 2) { //Проверка ФИО
                            bool valid_char = false;
                            if ((c >= 'а' && c <= 'я') || (c >= 'А' && c <= 'Я') ||
                                (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
                                c == ' ' || c == '-' || c == '\x2D') {
                                valid_char = true;
                            }

                            if (!valid_char) {
                                continue;
                            }
                        }
                        if (i < MAX_LENGTH - 1) {
                            text[current_col][i++] = c;
                            printf("%c", c);
                        }
                    }
                }
            }
        exit_organization_addresses:
            if (file) fclose(file); // Закрываем файл только если он был открыт.
            organization_addresses();
            return;
            break;
        }
        }
    } while (menu_out_case != 27);
}

void menu_choises() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    do {
        system("cls");
        for (int i = 6; i != 0; i--) printf("\n");
        tabul(11); printf("МЕНЮ ВЫБОРА ТИПА ДАННЫХ\n\n");
        tabul(11); printf("1)   ИСХОДЯЩАЯ КОРРЕСПОНДЕНЦИЯ\n");
        tabul(11); printf("2)   АДРЕСА ОРГАНИЗАЦИЙ\n");
        tabul(11); printf("Esc) ВЫХОД В МЕНЮ\n\n");
        tabul(11); printf("_____________________________________________\n\n");
        tabul(11); printf("ВЫБРАННЫЙ ПУТЬ К ПАПКЕ: %s\n", folder_way);
        tabul(11);
        menu_out = _getch();
        switch (menu_out) {
        case '1':
            if (folder_way[0] != '\0') {
                outgoing_correspondence();
                break;
            }
        case '2':
            if (folder_way[0] != '\0') {
                organization_addresses();
                break;
            }
        }
    } while (menu_out != 27);
}

void program_way() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    printf("\n");
    char folder_way_new[256] = { 0 };

    tabul(11); printf("ВВЕДИТЕ ПУТЬ К ПАПКЕ: ");
    scanf("%255[^\n]", folder_way_new);
    getchar();


    size_t len = strlen(folder_way_new);
    if (len > 0 && folder_way_new[len - 1] != '\\') {
        folder_way_new[len] = '\\';
        folder_way_new[len + 1] = '\0';
    }

    struct stat program_way_buffer;
    if (stat(folder_way_new, &program_way_buffer) == 0) {
        tabul(11); printf("ПУТЬ К ПАПКЕ ВЫБРАН. ");
        strcpy(folder_way, folder_way_new); // Копируем новый путь в глобальную переменную
    }
    else {
        tabul(11); printf("1Х1: ОШИБКА ВВОДА. НЕВЕРНЫЙ ПУТЬ К ПАПКЕ. ");
        memset(folder_way_new, 0, sizeof(folder_way_new));
    }
    printf("ДЛЯ ПРОДОЛЖЕНИЯ НАЖМИТЕ ENTER."); system("PAUSE>nul");
}

void menu() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    do {
        system("cls");
        for (int i = 3; i != 0; i--) printf("\n");
        tabul(11); printf("МЕНЮ\n\n");
        tabul(11); printf("1)   НАЧАТЬ РАБОТУ С ДАННЫМИ\n");
        tabul(11); printf("2)   ПУТЬ К ПАПКЕ ХРАНЕНИЯ ФАЙЛОВ\n");
        tabul(11); printf("3)   ИНСТРУКЦИЯ\n");
        tabul(11); printf("Esc) ВЫХОД ИЗ ПРОГРАММЫ\n\n");
        tabul(11); printf("_____________________________________________\n\n");
        tabul(11); printf("ВЫБРАННЫЙ ПУТЬ К ПАПКЕ: %s\n", folder_way);
        tabul(11);

        menu_out = _getch();
        switch (menu_out) {
        case '1':
            if (folder_way[0] != '\0')
            {
                menu_choises();
                break;
            }
            else
            {
                printf("Сначала выберите путь к папке. ");
                printf("ДЛЯ ПРОДОЛЖЕНИЯ НАЖМИТЕ ENTER.");
                system("PAUSE>nul");
                break;
            }

        case '2':
            program_way();
            break;
        case '3':
            instruction();
            break;
        }
    } while (menu_out != 27);
}