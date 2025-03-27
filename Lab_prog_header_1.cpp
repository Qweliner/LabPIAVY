//Lab_prog_header_1.cpp
#include "Lab_prog_header_1.h"
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

// Глобальные переменные (определения)
char folder_way[256] = { 0 };       // Путь к папке
const char* file_extension = ".txt";
const char* ocfe = "IC_";
const char* oa = "AO_";

// Функция проверки символа для имени файла
bool isValidFileNameChar(char c) {
    unsigned char uc = (unsigned char)c; // Для корректной работы с кириллицей
    return (uc != '\\' && uc != '/' && uc != ':' && uc != '*' && uc != '?' && uc != '"' && uc != '<' && uc != '>' && uc != '|');
}

// Функция проверки символа для пути
bool isValidPathChar(char c) {
    unsigned char uc = (unsigned char)c;
    return (uc != '*' && uc != '?' && uc != '"' && uc != '<' && uc != '>' && uc != '|');
}

// Функция для получения строки с консоли с фильтрацией символов
void getLineWithRestrictedChars(const char* instruction, char* buffer, int buffer_size, bool (*isValidCharFunc)(char)) {
    printf("%s", instruction);
    int i = 0;
    buffer[0] = '\0'; // Инициализация буфера

    while (true) {
        int key = _getch();
        if (key == 0 || key == 224) { // Расширенные клавиши
            if (_kbhit()) { // Проверяем, есть ли второй байт
                _getch();   // Игнорируем второй байт
                continue;
            }
        }
        if (key == 27) { // ESC
            buffer[0] = '\0';
            printf("\n");
            return;
        }
        else if (key == '\r') { // Enter
            buffer[i] = '\0';
            printf("\n");
            return;
        }
        else if (key == 8) { // Backspace
            if (i > 0) {
                i--;
                printf("\b \b");
                buffer[i] = '\0';
            }
        }
        else if (key == 22) { // Ctrl+V
            if (OpenClipboard(NULL)) {
                HANDLE hData = GetClipboardData(CF_TEXT);
                if (hData != NULL) {
                    char* pszText = (char*)GlobalLock(hData);
                    if (pszText != NULL) {
                        for (int j = 0; pszText[j] != '\0' && i < buffer_size - 1; ++j) {
                            // Проверяем вставляемый символ с помощью переданной функции
                            if (isValidCharFunc(pszText[j])) {
                                buffer[i++] = pszText[j];
                                printf("%c", pszText[j]);
                            }
                        }
                        GlobalUnlock(hData);
                    }
                }
                CloseClipboard();
            }
        }
        else { // Обычный символ
            char c = (char)key;
            // Проверяем символ с помощью переданной функции
            if (isValidCharFunc(c)) {
                if (i < buffer_size - 1) {
                    buffer[i] = c;
                    printf("%c", c);
                    i++;
                }
            }
            // Если символ невалидный, ничего не делаем (не печатаем, не добавляем)
        }
    }
}

// Функция проверки года на високосность
bool is_leap(int year) {
    return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
}

// Функция проверки корректности даты
bool isValidDate(const char* dateStr) {
    if (strcmp(dateStr, "нет данных") == 0) {
        return true; // "нет данных" всегда валидно
    }

    // Проверка формата ДД.ММ.ГГГГ (длина и разделители)
    if (strlen(dateStr) != 10 || dateStr[2] != '.' || dateStr[5] != '.') {
        return false; // Неправильный формат
    }

    int day, month, year;
    // Проверка, что все символы - цифры или точки
    for (int i = 0; dateStr[i] != '\0'; ++i) {
        if (!isdigit(dateStr[i]) && dateStr[i] != '.') {
            return false; // Неправильный формат
        }
    }

    // Пытаемся разобрать дату
    int count = sscanf(dateStr, "%d.%d.%d", &day, &month, &year);
    if (count != 3) {
        return false; // Неправильный формат
    }

    // Проверка корректности значений дня, месяца, года
    if (year < 1 || year > 9999 || month < 1 || month > 12 || day < 1) {
        return false; // Неправильная дата
    }

    int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (is_leap(year)) {
        daysInMonth[2] = 29;
    }

    if (day > daysInMonth[month]) {
        return false; // Неправильная дата (день)
    }

    return true; // Дата корректна
}

// Функция вывода инструкции
void instruction() {
    system("cls");

    FILE* file = fopen("instruction.txt", "r");
    if (file) {
        char line[256];
        while (fgets(line, sizeof(line), file)) {
            printf("%s", line);
        }
        fclose(file);
    }
    else {
        printf("Не удалось открыть файл с инструкцией. Убедитесь, что файл 'instruction.txt' находится в той же папке, что и программа.\n");
        char current_path[256];
        if (_getcwd(current_path, sizeof(current_path)) != NULL) {
            printf("Текущая папка: %s\n", current_path);
        }
        else
        {
            printf("Не удалось определить текущую папку.\n");
        }
    }
    printf("\nНажмите любую клавишу для продолжения...\n");
    _getch();
    system("cls");
}

// Функция открытия файла для дозаписи (или создания, если не существует)
// Убраны параметры mode, т.к. режим всегда "a" (append)
void openFileForAppend(FILE*& file, const char* full_name, const char* headers) {

    file = fopen(full_name, "a");
    if (!file) {
        printf("Не удалось открыть файл для записи. Возможно, файл занят другой программой, или у вас нет прав доступа.\n");
        printf("Для продолжения нажмите Enter.");
        system("PAUSE>nul");
        return;
    }

    system("cls");
    if (ftell(file) == 0) { // Проверяем, пустой ли файл (был только что создан)
        printf("Файл '%s' создан.\n\n", full_name);
        //fprintf(file, "%s", headers); // Записываем заголовки, если файл новый
        fflush(file);
    }
    else {
        printf("Файл '%s' открыт для дозаписи.\n\n", full_name);
    }

    printf("Путь к файлу: %s\n\n", full_name);
    printf("Нажмите TAB или Enter для перехода к следующему параметру.\n");
    printf("Нажмите ESC для выхода из режима ввода.\n");
    printf("Вы можете ввести 'нет данных' в любое поле (кроме даты - там только 'нет данных').\n");


    // Размер 256 выбран как компромисс между возможностью обрабатывать длинные строки
    // и экономией памяти, согласуется с размером буфера MAX_PATH в Windows.
    char fields[3][256];
    int current_col = 0;

    while (true) {
        for (current_col = 0; current_col < 3; current_col++) {
            int i = 0;
            memset(fields[current_col], 0, sizeof(fields[current_col]));

            // Вывод названия текущего поля
            if (current_col == 0) {
                if (strcmp(headers, "Вид корреспонденции; Дата подготовки; Название организации;\n") == 0) printf("\nВид корреспонденции: "); else printf("\nНазвание организации: ");
            }
            else if (current_col == 1) {
                if (strcmp(headers, "Вид корреспонденции; Дата подготовки; Название организации;\n") == 0) printf("\nДата подготовки (ДД.ММ.ГГГГ или нет данных): "); else printf("\nАдрес: ");
            }
            else if (current_col == 2) {
                if (strcmp(headers, "Вид корреспонденции; Дата подготовки; Название организации;\n") == 0) printf("\nНазвание организации: "); else printf("\nФамилия руководителя: ");
            }

            // Цикл ввода для текущего поля с проверкой символов
            while (true) {
                int key = _getch();
                char c = (char)key;
                bool allowed = false;
                if (key == 0 || key == 224) { // Расширенные клавиши
                    if (_kbhit()) { // Проверяем, есть ли второй байт
                        _getch();   // Игнорируем второй байт
                        continue;
                    }
                }
                // Определяем, разрешен ли символ ДЛЯ ТЕКУЩЕГО ПОЛЯ
                unsigned char uc = (unsigned char)c; // Для корректной работы с кириллицей
                bool is_letter = (uc >= 'A' && uc <= 'Z') || (uc >= 'a' && uc <= 'z') || (uc >= 192); // Латиница + Кириллица (грубо)
                bool is_digit = (uc >= '0' && uc <= '9');
                bool is_space = (uc == ' ');

                if (current_col == 0) { // Вид корреспонденции / Название организации (первое поле)
                    if (strcmp(headers, "Вид корреспонденции; Дата подготовки; Название организации;\n") == 0) { // Вид корреспонденции
                        allowed = is_letter || is_digit || is_space || uc == '-' || uc == '_' || uc == '.' || uc == ',' || uc == '(' || uc == ')';
                    }
                    else { // Название организации (из адресов)
                        allowed = is_letter || is_digit || is_space || uc == '-' || uc == '_' || uc == '.' || uc == ',' || uc == '"' || uc == '(' || uc == ')' || uc == 211 || uc == '+' || uc == '!' || uc == '&' || uc == ':' || uc == 171 || uc == 187 || uc == '#'; // №(211), «(171), »(187)
                    }
                }
                else if (current_col == 1) { // Дата подготовки / Адрес
                    if (strcmp(headers, "Вид корреспонденции; Дата подготовки; Название организации;\n") == 0) { // Дата подготовки
                        allowed = is_digit || uc == '.' || is_letter || is_space; // Разрешаем буквы и пробел для "нет данных"
                    }
                    else { // Адрес
                        allowed = is_letter || is_digit || is_space || uc == '-' || uc == ',' || uc == '.' || uc == '/' || uc == 211; // №(211)
                    }
                }
                else if (current_col == 2) { // Название организации / Фамилия руководителя
                    if (strcmp(headers, "Вид корреспонденции; Дата подготовки; Название организации;\n") == 0) { // Название организации (из корреспонденции)
                        allowed = is_letter || is_digit || is_space || uc == '-' || uc == '_' || uc == '.' || uc == ',' || uc == '"' || uc == '(' || uc == ')' || uc == 211 || uc == '+' || uc == '!' || uc == '&' || uc == ':' || uc == 171 || uc == 187 || uc == '#'; // №(211), «(171), »(187)
                    }
                    else { // Фамилия руководителя
                        allowed = is_letter || uc == '-' || is_space;
                    }
                }

                // Обработка клавиш управления
                if (key == 27) { // ESC
                    if (file) fclose(file);
                    printf("\nВвод прерван.\n");
                    return;
                }
                else if (key == '\t' || key == '\r') { // Tab or Enter
                    fields[current_col][i] = '\0';

                    // Проверка даты после ввода
                    if (current_col == 1 && strcmp(headers, "Вид корреспонденции; Дата подготовки; Название организации;\n") == 0) {
                        if (!isValidDate(fields[current_col])) {
                            // Определяем тип ошибки даты
                            bool format_error = true;
                            if (strlen(fields[current_col]) == 10 && fields[current_col][2] == '.' && fields[current_col][5] == '.') {
                                int d, m, y;
                                if (sscanf(fields[current_col], "%d.%d.%d", &d, &m, &y) == 3) {
                                    format_error = false; // Формат верный, значит дата некорректна
                                }
                            }
                            if (format_error) {
                                printf("\nНеправильный формат даты. Введите дату в формате 'ДД.ММ.ГГГГ' (например, 01.01.2023) или введите 'нет данных'.\n");
                            }
                            else {
                                printf("\nНеправильная дата. Проверьте, что введенные день, месяц и год существуют. Например, 31.02.2023 - некорректная дата.\n");
                            }
                            i = 0;
                            memset(fields[current_col], 0, sizeof(fields[current_col]));
                            // Заново выводим приглашение для ввода даты
                            printf("\nДата подготовки (ДД.ММ.ГГГГ или нет данных): ");
                            continue; // Продолжаем ввод для этого же поля
                        }
                    }

                    // Проверка на пустой ввод
                    if (i == 0 && strcmp(fields[current_col], "нет данных") != 0 && !(current_col == 1 && strcmp(headers, "Вид корреспонденции; Дата подготовки; Название организации;\n") == 0 && fields[current_col][0] == '\0'))
                    {
                        printf("\nВы не ввели никаких данных. Пожалуйста, введите информацию или укажите 'нет данных'.\n");
                        // Заново выводим приглашение для ввода
                        if (current_col == 0) { if (strcmp(headers, "Вид корреспонденции; Дата подготовки; Название организации;\n") == 0) printf("\nВид корреспонденции: "); else printf("\nНазвание организации: "); }
                        else if (current_col == 1) { if (strcmp(headers, "Вид корреспонденции; Дата подготовки; Название организации;\n") == 0) printf("\nДата подготовки (ДД.ММ.ГГГГ или нет данных): "); else printf("\nАдрес: "); }
                        else if (current_col == 2) { if (strcmp(headers, "Вид корреспонденции; Дата подготовки; Название организации;\n") == 0) printf("\nНазвание организации: "); else printf("\nФамилия руководителя: "); }
                        continue; // Продолжаем ввод для этого же поля
                    }
                    break; // Переход к следующему полю
                }
                else if (key == 8) { // Backspace
                    if (i > 0) {
                        i--;
                        printf("\b \b");
                        fields[current_col][i] = '\0';
                    }
                }
                else if (key == 22) { // Ctrl+V
                    if (OpenClipboard(NULL)) {
                        HANDLE hData = GetClipboardData(CF_TEXT);
                        if (hData != NULL) {
                            char* pszText = (char*)GlobalLock(hData);
                            if (pszText != NULL) {
                                for (int j = 0; pszText[j] != '\0' && i < sizeof(fields[current_col]) - 1; ++j) {
                                    // Проверяем вставляемый символ на допустимость для ТЕКУЩЕГО ПОЛЯ
                                    unsigned char uct = (unsigned char)pszText[j];
                                    bool allowed_paste = false;
                                    bool is_letter_p = (uct >= 'A' && uct <= 'Z') || (uct >= 'a' && uct <= 'z') || (uct >= 192);
                                    bool is_digit_p = (uct >= '0' && uct <= '9');
                                    bool is_space_p = (uct == ' ');

                                    if (current_col == 0) { if (strcmp(headers, "Вид корреспонденции; Дата подготовки; Название организации;\n") == 0) { allowed_paste = is_letter_p || is_digit_p || is_space_p || uct == '-' || uct == '_' || uct == '.' || uct == ',' || uct == '(' || uct == ')'; } else { allowed_paste = is_letter_p || is_digit_p || is_space_p || uct == '-' || uct == '_' || uct == '.' || uct == ',' || uct == '(' || uct == ')' || uct == 211 || uct == '+' || uct == '!' || uct == '&' || uct == ':' || uct == 171 || uct == 187 || uct == '#'; } }
                                    else if (current_col == 1) { if (strcmp(headers, "Вид корреспонденции; Дата подготовки; Название организации;\n") == 0) { allowed_paste = is_digit_p || uct == '.' || is_letter_p || is_space_p; } else { allowed_paste = is_letter_p || is_digit_p || is_space_p || uct == '-' || uct == ',' || uct == '.' || uct == '/' || uct == 211; } }
                                    else if (current_col == 2) { if (strcmp(headers, "Вид корреспонденции; Дата подготовки; Название организации;\n") == 0) { allowed_paste = is_letter_p || is_digit_p || is_space_p || uct == '-' || uct == '_' || uct == '.' || uct == ',' || uct == '(' || uct == ')' || uct == 211 || uct == '+' || uct == '!' || uct == '&' || uct == ':' || uct == 171 || uct == 187 || uct == '#'; } else { allowed_paste = is_letter_p || uct == '-' || is_space_p; } }

                                    if (allowed_paste) {
                                        fields[current_col][i++] = pszText[j];
                                        printf("%c", pszText[j]);
                                    }
                                }
                                GlobalUnlock(hData);
                            }
                        }
                        CloseClipboard();
                    }
                }
                else { // Обычный символ
                    if (allowed && i < sizeof(fields[current_col]) - 1) {
                        fields[current_col][i] = c;
                        printf("%c", c);
                        i++;
                    }
                    // Если символ не разрешен (allowed == false), ничего не делаем
                }
            } // конец while(true) для ввода одного поля
        } // конец for по полям (current_col)

        if (strcmp(fields[0], "нет данных") == 0 &&
            strcmp(fields[1], "нет данных") == 0 &&
            strcmp(fields[2], "нет данных") == 0)
        {
            // Если все поля "нет данных", выводим сообщение и не записываем
            printf("\nВсе поля содержат 'нет данных'. Запись не будет добавлена.\n");
        }
        else
        {
            // Если хотя бы одно поле не "нет данных", записываем строку
            fprintf(file, "%s; %s; %s;\n", fields[0], fields[1], fields[2]);
            fflush(file); // Сбрасываем буфер файла на диск (для надежности)
            printf("\nДанные сохранены. Введите следующую запись или нажмите ESC для выхода.\n");
        }
    }
}

// Функция для работы с исходящей корреспонденцией
void outgoing_correspondence() {
    char txt_name[256];
    char full_name[512];

    do {
        system("cls");
        SetConsoleCP(1251);
        SetConsoleOutputCP(1251);
        printf("Исходящая корреспонденция:\n");
        printf("1.   Создать/открыть файл для дозаписи\n");
        printf("2.   Перезаписать файл\n");
        printf("Esc. Выход в выбор типа файла\n\n");
        printf("_____________________________________________\n\n");
        printf("Текущий путь: %s\n\n", folder_way);

        char menu_out_case = _getch();
        if (menu_out_case == 27) {
            return;
        }

        FILE* file;
        switch (menu_out_case) {
        case '1': {
            printf("\n");
            // Используем getLineWithRestrictedChars с проверкой для имени файла
            getLineWithRestrictedChars("Введите имя файла: ", txt_name, sizeof(txt_name), isValidFileNameChar);
            if (txt_name[0] == '\0') { // Если пользователь нажал Esc
                break;
            }
            // Дополнительная проверка на пустой ввод после getLine...
            if (strlen(txt_name) == 0) {
                printf("Имя файла не может быть пустым.\n");
                printf("Для продолжения нажмите Enter."); system("PAUSE>nul");
                break;
            }


            snprintf(full_name, sizeof(full_name), "%s%s%s%s", folder_way, ocfe, txt_name, file_extension);
            openFileForAppend(file, full_name, "Вид корреспонденции; Дата подготовки; Название организации;\n");
            break;
        }
        case '2': {
            printf("\n");
            getLineWithRestrictedChars("Введите имя файла для перезаписи: ", txt_name, sizeof(txt_name), isValidFileNameChar);
            if (txt_name[0] == '\0') { // Esc
                break;
            }
            if (strlen(txt_name) == 0) {
                printf("Имя файла не может быть пустым.\n");
                printf("Для продолжения нажмите Enter."); system("PAUSE>nul");
                break;
            }

            snprintf(full_name, sizeof(full_name), "%s%s%s%s", folder_way, ocfe, txt_name, file_extension);

            bool exists = (_access(full_name, 0) == 0);
            if (exists) {
                printf("Файл с таким именем уже существует. Перезаписать? (y/n): ");
                char overwriteConfirm = _getch();
                printf("\n"); // Перевод строки после _getch
                if (tolower(overwriteConfirm) != 'y') {
                    printf("Файл не был перезаписан.\n");
                    printf("Для продолжения нажмите Enter."); system("PAUSE>nul");
                    break;
                }
            }

            file = fopen(full_name, "w"); // Открываем для перезаписи ("w")
            if (file) {
                fclose(file); // Закрываем, чтобы openFileForAppend мог открыть в "a"
                printf("Файл '%s' успешно %s.\n", full_name, exists ? "перезаписан" : "создан");
                // Открываем для добавления данных (заголовки добавятся автоматически)
                openFileForAppend(file, full_name, "Вид корреспонденции; Дата подготовки; Название организации;\n");
            }
            else {
                printf("Не удалось открыть файл для записи. Возможно, файл занят другой программой, или у вас нет прав доступа.\n");
                printf("Для продолжения нажмите Enter."); system("PAUSE>nul");
            }
            break;
        }
        default:
            // Некорректный ввод в меню обрабатывается циклом do-while
            break;
        }
    } while (true);
}

// Функция для работы с адресами организаций
void organization_addresses() {
    char txt_name[256];
    char full_name[512];

    do {
        system("cls");
        SetConsoleCP(1251);
        SetConsoleOutputCP(1251);
        printf("Адреса организаций:\n");
        printf("1.   Создать/открыть файл для дозаписи\n");
        printf("2.   Перезаписать файл\n");
        printf("Esc. Выход в выбор типа файла\n\n");
        printf("_____________________________________________\n\n");
        printf("Текущий путь: %s\n\n", folder_way);
        char menu_out_case = _getch();
        if (menu_out_case == 27) {
            return;
        }

        FILE* file;
        switch (menu_out_case) {
        case '1': {
            printf("\n");
            getLineWithRestrictedChars("Введите имя файла: ", txt_name, sizeof(txt_name), isValidFileNameChar);
            if (txt_name[0] == '\0') { // Esc
                break;
            }
            if (strlen(txt_name) == 0) {
                printf("Имя файла не может быть пустым.\n");
                printf("Для продолжения нажмите Enter."); system("PAUSE>nul");
                break;
            }

            snprintf(full_name, sizeof(full_name), "%s%s%s%s", folder_way, oa, txt_name, file_extension);
            openFileForAppend(file, full_name, "Название организации; Адрес; Фамилия руководителя;\n");
            break;
        }
        case '2': {
            printf("\n");
            getLineWithRestrictedChars("Введите имя файла для перезаписи: ", txt_name, sizeof(txt_name), isValidFileNameChar);
            if (txt_name[0] == '\0') { // Esc
                break;
            }
            if (strlen(txt_name) == 0) {
                printf("Имя файла не может быть пустым.\n");
                printf("Для продолжения нажмите Enter."); system("PAUSE>nul");
                break;
            }

            snprintf(full_name, sizeof(full_name), "%s%s%s%s", folder_way, oa, txt_name, file_extension);

            bool exists = (_access(full_name, 0) == 0);
            if (exists) {
                printf("Файл с таким именем уже существует. Перезаписать? (y/n): ");
                char overwriteConfirm = _getch();
                printf("\n");
                if (tolower(overwriteConfirm) != 'y') {
                    printf("Файл не был перезаписан.\n");
                    printf("Для продолжения нажмите Enter."); system("PAUSE>nul");
                    break;
                }
            }

            file = fopen(full_name, "w");
            if (file) {
                fclose(file);
                printf("Файл '%s' успешно %s.\n", full_name, exists ? "перезаписан" : "создан");
                openFileForAppend(file, full_name, "Название организации; Адрес; Фамилия руководителя;\n");
            }
            else {
                printf("Не удалось открыть файл для записи. Возможно, файл занят другой программой, или у вас нет прав доступа.\n");
                printf("Для продолжения нажмите Enter."); system("PAUSE>nul");
            }
            break;
        }
        default:
            break;
        }
    } while (true);
}

// Функция меню выбора типа данных
void menu_choises() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    do {
        system("cls");
        printf("Меню выбора типа данных:\n");
        printf("1.   Исходящая корреспонденция\n");
        printf("2.   Адреса организаций\n");
        printf("Esc. Выход в меню\n\n");
        printf("_____________________________________________\n\n");
        printf("Выбранный путь к папке: %s\n", folder_way);

        char menu_out = _getch();
        if (menu_out == 27) {
            system("cls");
            return;
        }
        switch (menu_out) {
        case '1':
            if (folder_way[0] != '\0') {
                outgoing_correspondence();
            }
            else {
                printf("Сначала выберите путь к папке в главном меню (пункт 2).\n");
                printf("Для продолжения нажмите Enter."); system("PAUSE>nul");
            }
            break;
        case '2':
            if (folder_way[0] != '\0') {
                organization_addresses();
            }
            else {
                printf("Сначала выберите путь к папке в главном меню (пункт 2).\n");
                printf("Для продолжения нажмите Enter."); system("PAUSE>nul");
            }
            break;

        default:
            break;
        }
    } while (true);
}

// Функция нормализации пути (упрощенная для Windows)
void normalizePath(const char* path, char* normalized_path, size_t normalized_path_size) {
    if (path[0] == '\0') {
        if (_getcwd(normalized_path, normalized_path_size) == NULL) {
            fprintf(stderr, "Не удалось определить текущую папку.\n");
            normalized_path[0] = '\0';
        }
        return;
    }

    // Используем _fullpath для нормализации под Windows
    if (_fullpath(normalized_path, path, normalized_path_size) == NULL) {
        // Ошибка нормализации (например, путь слишком длинный или недопустимый)
        // В normalized_path может быть мусор, очистим его
        SAFE_STRCPY(normalized_path, path, normalized_path_size); // Копируем как есть для дальнейшей проверки
        // Можно добавить вывод ошибки, но stat потом все равно проверит
    }

    // Дополнительно убираем повторяющиеся слеши, т.к. _fullpath их может оставить
    int j = 0;
    bool last_was_slash = false;
    for (int i = 0; normalized_path[i] != '\0'; ++i) {
        if (normalized_path[i] == '\\') {
            if (!last_was_slash) {
                normalized_path[j++] = normalized_path[i];
            }
            last_was_slash = true;
        }
        else {
            normalized_path[j++] = normalized_path[i];
            last_was_slash = false;
        }
    }
    normalized_path[j] = '\0';

    // Убираем завершающий слеш, если он не является частью корневого пути (C:\)
    size_t len = strlen(normalized_path);
    if (len > 3 && normalized_path[len - 1] == '\\') {
        normalized_path[len - 1] = '\0';
    }
}

// Функция выбора пути к папке
void program_way() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    printf("\n");
    char folder_way_new[256] = { 0 };
    char normalized_path_temp[256]; // Временный буфер для нормализации

    // Ввод пути с проверкой символов
    getLineWithRestrictedChars("Введите путь к папке: ", folder_way_new, sizeof(folder_way_new), isValidPathChar);

    if (folder_way_new[0] == '\0') { // Пользователь нажал Esc
        return;
    }

    // Нормализация пути
    normalizePath(folder_way_new, normalized_path_temp, sizeof(normalized_path_temp));

    // Проверка существования пути и прав доступа
    struct stat path_stat;
    if (stat(normalized_path_temp, &path_stat) == 0) { // Путь существует
        if (path_stat.st_mode & S_IFDIR) { // Это директория
            // Проверка прав на запись (пытаемся создать и удалить тестовый файл)
            char test_file_path[512];
            // Добавляем слеш к нормализованному пути для snprintf, если его нет
            size_t len_norm = strlen(normalized_path_temp);
            char path_with_slash[257]; // +1 для слеша и +1 для нуля
            SAFE_STRCPY(path_with_slash, normalized_path_temp, sizeof(path_with_slash));
            if (len_norm > 0 && path_with_slash[len_norm - 1] != '\\') {
                if (len_norm < sizeof(path_with_slash) - 1) {
                    path_with_slash[len_norm] = '\\';
                    path_with_slash[len_norm + 1] = '\0';
                }
                else {
                    // Путь слишком длинный для добавления слеша, вероятно ошибка
                    printf("Указанный путь не существует. Проверьте правильность написания и убедитесь, что папка существует.\n");
                    if (folder_way[0] != '\0') printf("Используется последний корректный путь: %s\n", folder_way);
                    printf("Для продолжения нажмите Enter."); system("PAUSE>nul");
                    return;
                }
            }

            snprintf(test_file_path, sizeof(test_file_path), "%stest_access.tmp", path_with_slash); // Используем временное расширение

            FILE* test_file = fopen(test_file_path, "w");
            if (test_file) { // Права на запись есть
                fclose(test_file);
                remove(test_file_path); // Удаляем тестовый файл
                printf("Путь к папке выбран: %s\n", path_with_slash);
                SAFE_STRCPY(folder_way, path_with_slash, sizeof(folder_way)); // Сохраняем путь со слешем
            }
            else { // Нет прав на запись
                printf("Нет прав для доступа к указанной папке. Выберите другую папку или обратитесь к администратору.\n");
                if (folder_way[0] != '\0') printf("Используется последний корректный путь: %s\n", folder_way);
            }
        }
        else { // Путь существует, но это не директория
            printf("Указанный путь не является папкой.\n");
            if (folder_way[0] != '\0') printf("Используется последний корректный путь: %s\n", folder_way);
        }
    }
    else { // Путь не существует
        printf("Указанный путь не существует. Проверьте правильность написания и убедитесь, что папка существует.\n");
        if (folder_way[0] != '\0') printf("Используется последний корректный путь: %s\n", folder_way);
    }

    printf("Для продолжения нажмите Enter."); system("PAUSE>nul");
}

// Главное меню программы
void menu() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // Автоматическое определение пути при запуске
    if (folder_way[0] == '\0') {
        char current_path[256];
        if (_getcwd(current_path, sizeof(current_path)) != NULL)
        {
            // Добавляем слеш в конце
            size_t len = strlen(current_path);
            if (len > 0 && current_path[len - 1] != '\\' && len < sizeof(folder_way) - 1) {
                current_path[len] = '\\';
                current_path[len + 1] = '\0';
            }
            SAFE_STRCPY(folder_way, current_path, sizeof(folder_way));
        }
        else
        {
            fprintf(stderr, "Не удалось определить текущую папку.\n");
        }
    }

    printf("\nПрограмма для ведения базы данных о корреспонденции и адресах организаций.\n");

    while (true) {
        system("cls");

        printf("*************************************************************\n");
        printf("*    Программа для ведения базы данных о корреспонденции    *\n");
        printf("*                 и адресах организаций.                    *\n");
        printf("*      Позволяет добавлять, изменять и удалять данные.      *\n");
        printf("*************************************************************\n");

        printf("Главное меню:\n");
        printf("1.   Начать работу\n");
        printf("2.   Изменить путь к папке\n");
        printf("3.   Инструкция\n");
        printf("Esc. Выход\n");
        printf("_____________________________________________\n\n");
        printf("Текущий путь: %s\n\n", folder_way);


        char c = _getch();
        switch (c) {
        case '1':
            if (folder_way[0] != '\0') {
                menu_choises();
            }
            else {
                printf("Сначала выберите путь к папке (пункт 2).\n");
                printf("Для продолжения нажмите Enter."); system("PAUSE>nul");
            }
            break;

        case '2':
            program_way();
            break;
        case '3':
            instruction();
            break;
        case 27: // ESC
            return;
        default:
            // Игнорируем неверный ввод в меню
            break;
        }
    }
}