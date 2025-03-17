#include "Lab_prog_header_1.h"
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

// Глобальные переменные (определения)
char folder_way[256] = { 0 };       // Путь к папке
const char* file_extension = ".txt";
const char* ocfe = "IC_";
const char* oa = "AO_";

// Макрос для безопасного копирования строк
#define SAFE_STRCPY(dest, src, size) do { strncpy(dest, src, size - 1); dest[size - 1] = '\0'; } while(0)

// Функция для получения строки с консоли с обработкой Esc и Backspace
void getLineWithEsc(const char* instruction, char* buffer, int buffer_size) {
    printf("%s", instruction);
    int i = 0;
    while (true) {
        int key = _getch();
        if (key == 27) { // ESC
            buffer[0] = '\0'; // Очистка буфера
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
                buffer[i] = '\0'; // Обновляем буфер
            }
        }
        else if (key >= 32 && key <= 255) { // Расширенный диапазон для кириллицы
            if (i < buffer_size - 1) { // Проверка границ
                buffer[i] = (char)key;
                printf("%c", (char)key);
                i++;
            }
        }
        else if (key == 0 || key == 224) { // Обработка специальных клавиш
            _getch();
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
        return true;
    }
    if (strlen(dateStr) != 10 && strcmp(dateStr, "нет данных") != 0) {
        return false;
    }

    int day, month, year;
    int count = sscanf(dateStr, "%d.%d.%d", &day, &month, &year);

    if (count != 3 && strcmp(dateStr, "нет данных") != 0) {
        return false;
    }
    if (strcmp(dateStr, "нет данных") == 0)
    {
        return true;
    }
    if (month < 1 || month > 12) {
        return false;
    }
    if (day < 1 || day > 31) {
        return false;
    }
    if (month == 2) {
        int daysInFeb = 28 + (is_leap(year) ? 1 : 0);
        if (day > daysInFeb) {
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

// Функция проверки имени файла на недопустимые символы
bool isValidFileName(const char* fileName) {
    const char* invalidChars = "\\/:*?\"<>|"; // Запрещенные символы

    // Проверяем, нет ли в имени файла запрещенных символов
    for (int i = 0; fileName[i] != '\0'; ++i) {
        for (int j = 0; invalidChars[j] != '\0'; ++j) {
            if (fileName[i] == invalidChars[j]) {
                return false; // Найден запрещенный символ
            }
        }
    }

    return true; // Запрещенных символов нет
}

// Функция вывода инструкции
void instruction() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    system("cls");

    FILE* file = fopen("instruction.txt", "r");
    if (file) {
        char line[256]; // Буфер для чтения строки
        while (fgets(line, sizeof(line), file)) {
            printf("%s", line);
        }
        fclose(file);
    }
    else {
        printf("Не удалось открыть файл instruction.txt\n");
        printf("Пожалуйста, поместите файл instruction.txt в следующую папку:\n");
        char current_path[256];
        if (_getcwd(current_path, sizeof(current_path)) != NULL) {
            printf("%s\n", current_path);
        }
        else
        {
            printf("Ошибка при получении текущего пути.\n");
        }
    }
    printf("\nНажмите любую клавишу для продолжения...\n");
    _getch();
    system("cls");
}

// Функция открытия файла для дозаписи (или создания, если не существует)
void openFileForAppend(FILE*& file, const char* full_name, const char* headers, char* mode, size_t mode_size) {

    if (_access(full_name, 0) == 0) {
        SAFE_STRCPY(mode, "Открытие файла (дозапись)", mode_size);
    }
    else {
        SAFE_STRCPY(mode, "Создание файла", mode_size);
    }

    file = fopen(full_name, "a");
    if (!file) {
        printf("Ошибка открытия файла для дозаписи.\n");
        printf("Для продолжения нажмите Enter.");
        system("PAUSE>nul");
        return;
    }

    system("cls");
    printf("Файл открыт для дозаписи.\n\n");
    printf("Путь к файлу: %s\n\n", full_name);
    printf("Нажмите TAB для перехода к следующему параметру.\n");
    printf("Нажмите Enter для перехода к следующему параметру.\n");
    printf("Вы можете ввести 'нет данных' в любое поле.\n");


    char fields[3][1000]; // Массив для хранения трех полей
    int current_col = 0;

    while (true) {
        for (current_col = 0; current_col < 3; current_col++) {
            int i = 0;
            memset(fields[current_col], 0, sizeof(fields[current_col]));

            if (current_col == 0) {
                if (strcmp(headers, "Тип корреспонденции | Дата | Название организации\n") == 0) {
                    printf("\nТип корреспонденции: ");
                }
                else {
                    printf("\nНазвание организации: ");
                }
            }
            else if (current_col == 1) {
                if (strcmp(headers, "Тип корреспонденции | Дата | Название организации\n") == 0) {
                    printf("\nДата (ДД.ММ.ГГГГ): ");
                }
                else {
                    printf("\nАдрес: ");
                }
            }
            else if (current_col == 2) {
                if (strcmp(headers, "Тип корреспонденции | Дата | Название организации\n") == 0) {
                    printf("\nНазвание организации: ");
                }
                else {
                    printf("\nФИО организатора: ");
                }
            }

            while (true) {
                char c = _getch();

                if (c == 27) {
                    if (file) fclose(file);
                    return;
                }
                else if (c == '\t' || c == '\r') {
                    fields[current_col][i] = '\0';

                    if (current_col == 1 && strcmp(headers, "Тип корреспонденции | Дата | Название организации\n") == 0) {
                        if (!isValidDate(fields[current_col])) {
                            printf("\nНеверный формат даты. Пожалуйста, введите дату в формате ДД.ММ.ГГГГ или 'нет данных': \n");
                            i = 0;
                            memset(fields[current_col], 0, sizeof(fields[current_col]));
                            continue;
                        }
                    }

                    if (i == 0 && strcmp(fields[current_col], "нет данных") != 0)
                    {
                        printf("\nВы ничего не ввели. Повторите ввод.");
                        printf("\nЕсли у вас нет данных для заполнения то введите 'нет данных'.");
                        printf("\nИначе, введите необходимую информацию: \n");
                        continue;
                    }
                    break;
                }

                else if (c == 8) {
                    if (i > 0) {
                        i--;
                        printf("\b \b");
                        fields[current_col][i] = '\0';
                    }
                }
                else {
                    // Ограничение ввода для ФИО (и для других полей - разрешаем всё)
                    if (current_col == 2 && strcmp(headers, "Название организации | Адрес | ФИО организатора\n") == 0)
                    {
                        if (i < 999)  // проверка на переполнение буфера
                        {
                            if ((c >= 'а' && c <= 'я') || (c >= 'А' && c <= 'Я') ||
                                (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
                                c == ' ' || c == '-' || c == L'ё' || c == L'Ё')
                            {
                                fields[current_col][i++] = c;
                                printf("%c", c);
                            }
                        }
                    }
                    else
                    {
                        // Для всех остальных полей вводим всё
                        if (i < 999)
                        {
                            fields[current_col][i++] = c;
                            printf("%c", c);
                        }
                    }

                }
            }
        }
        // Запись в файл после ввода всех трех полей
        fprintf(file, "%s; %s; %s;\n", fields[0], fields[1], fields[2]);
        fflush(file); // Принудительная запись из буфера на диск
        printf("\n");
    }
}

// Функция для работы с исходящей корреспонденцией
void outgoing_correspondence() {
    char txt_name[256];
    char full_name[512];
    char mode[256]; // Буфер для строки "режим"

    do {
        system("cls");
        SetConsoleCP(1251);
        SetConsoleOutputCP(1251);
        printf("Исходящая корреспонденция:\n");
        printf("1.   Создать/открыть файл для дозаписи\n");
        printf("2.   Перезаписать файл\n");
        printf("Esc. Выход в выбор типа файла\n\n");
        printf("_____________________________________________\n\n");
        char menu_out_case = _getch();
        if (menu_out_case == 27) {
            return;
        }

        FILE* file;
        switch (menu_out_case) {
        case '1': {
            printf("\n");
            printf("Введите имя файла: ");
            getLineWithEsc("", txt_name, sizeof(txt_name));
            if (txt_name[0] == '\0') {
                break;
            }

            if (!isValidFileName(txt_name)) {
                printf("Недопустимые символы в имени файла.  Пожалуйста, используйте другое имя.\n");
                printf("Для продолжения нажмите Enter."); system("PAUSE>nul");
                break;
            }

            snprintf(full_name, sizeof(full_name), "%s%s%s%s", folder_way, ocfe, txt_name, file_extension);
            openFileForAppend(file, full_name, "Тип корреспонденции | Дата | Название организации\n", mode, sizeof(mode));
            break;
        }
        case '2': {
            printf("\n");
            printf("Введите имя файла для перезаписи: ");
            getLineWithEsc("", txt_name, sizeof(txt_name));

            if (txt_name[0] == '\0') {
                break;
            }
            if (!isValidFileName(txt_name)) {
                printf("Недопустимые символы в имени файла. Пожалуйста, используйте другое имя.\n");
                printf("Для продолжения нажмите Enter."); system("PAUSE>nul");
                break;
            }

            snprintf(full_name, sizeof(full_name), "%s%s%s%s", folder_way, ocfe, txt_name, file_extension);

            if (_access(full_name, 0) == 0) {
                printf("Файл с таким именем уже существует.  Перезаписать? (y/n): ");
                char overwriteConfirm = _getch();
                if (tolower(overwriteConfirm) != 'y') {
                    printf("\nФайл не был перезаписан.\n");
                    printf("Для продолжения нажмите Enter."); system("PAUSE>nul");
                    break;
                }
                printf("\n");
                SAFE_STRCPY(mode, "Перезапись файла", sizeof(mode));
            }
            else {
                SAFE_STRCPY(mode, "Создание файла", sizeof(mode));
            }

            file = fopen(full_name, "w");
            if (file) {
                fclose(file);
                printf("Файл успешно перезаписан.\n");
                openFileForAppend(file, full_name, "Тип корреспонденции | Дата | Название организации\n", mode, sizeof(mode));
            }
            else {
                printf("Ошибка перезаписи файла.\n");
                printf("Для продолжения нажмите Enter."); system("PAUSE>nul");
            }
            break;
        }
        default:
            printf("Некорректный ввод. Пожалуйста, выберите 1, 2 или Esc.\n");
            printf("Для продолжения нажмите Enter."); system("PAUSE>nul");
            break;
        }
    } while (true);
}

// Функция для работы с адресами организаций
void organization_addresses() {
    char txt_name[256];
    char full_name[512];
    char mode[256];

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
            printf("Введите имя файла: ");
            getLineWithEsc("", txt_name, sizeof(txt_name));
            if (txt_name[0] == '\0') {
                break;
            }

            if (!isValidFileName(txt_name)) {
                printf("Недопустимые символы в имени файла. Пожалуйста, используйте другое имя.\n");
                printf("Для продолжения нажмите Enter."); system("PAUSE>nul");
                break;
            }

            snprintf(full_name, sizeof(full_name), "%s%s%s%s", folder_way, oa, txt_name, file_extension);
            openFileForAppend(file, full_name, "Название организации | Адрес | ФИО организатора\n", mode, sizeof(mode));
            break;
        }
        case '2': {
            printf("\n");
            printf("Введите имя файла для перезаписи: ");
            getLineWithEsc("", txt_name, sizeof(txt_name));
            if (txt_name[0] == '\0') {
                break;
            }

            if (!isValidFileName(txt_name)) {
                printf("Недопустимые символы в имени файла.  Пожалуйста, используйте другое имя.\n");
                printf("Для продолжения нажмите Enter."); system("PAUSE>nul");
                break;
            }

            snprintf(full_name, sizeof(full_name), "%s%s%s%s", folder_way, oa, txt_name, file_extension);

            if (_access(full_name, 0) == 0) {
                printf("Файл с таким именем уже существует. Перезаписать? (y/n): ");
                char overwriteConfirm = _getch();
                if (tolower(overwriteConfirm) != 'y') {
                    printf("\nФайл не был перезаписан.\n");
                    printf("Для продолжения нажмите Enter."); system("PAUSE>nul");
                    break;
                }
                printf("\n");
                SAFE_STRCPY(mode, "Перезапись файла", sizeof(mode));
            }
            else {
                SAFE_STRCPY(mode, "Создание файла", sizeof(mode));
            }

            file = fopen(full_name, "w");
            if (file) {
                fclose(file);
                printf("Файл успешно перезаписан.\n");
                openFileForAppend(file, full_name, "Название организации | Адрес | ФИО организатора\n", mode, sizeof(mode));
            }
            else {
                printf("Ошибка перезаписи файла.\n");
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
                break;
            }
            else {
                printf("Сначала выберите путь к папке в главном меню (пункт 2).\n");
                printf("Для продолжения нажмите Enter."); system("PAUSE>nul");
            }
            break;
        case '2':
            if (folder_way[0] != '\0') {
                organization_addresses();
                break;
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

// Функция нормализации пути
void normalizePath(const char* path, char* normalized_path, size_t normalized_path_size) {
    char current_dir[256];

    if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
        fprintf(stderr, "Ошибка получения текущего каталога.\n");
        normalized_path[0] = '\0';
        return;
    }

    if (path[0] == '\0') {
        SAFE_STRCPY(normalized_path, current_dir, normalized_path_size);
        return;
    }

    // Абсолютный путь
    if ((isalpha(path[0]) && path[1] == ':') || path[0] == '\\' || path[0] == '/') {
        // Проверка на завершающий слеш во входном пути
        size_t input_len = strlen(path);
        if (input_len > 0 && (path[input_len - 1] == '\\' || path[input_len - 1] == '/')) {
            char temp_path[256]; // Временная переменная
            SAFE_STRCPY(temp_path, path, sizeof(temp_path));
            temp_path[input_len - 1] = '\0';
            SAFE_STRCPY(normalized_path, temp_path, normalized_path_size);
        }
        else {
            SAFE_STRCPY(normalized_path, path, normalized_path_size);
        }
        return;
    }
    // Относительный путь
    char temp_path[256] = { 0 };
    SAFE_STRCPY(temp_path, current_dir, sizeof(temp_path));

    char* token = strtok((char*)path, "\\/");
    while (token != NULL) {
        if (strcmp(token, ".") == 0) {
            // Текущая директория
        }
        else if (strcmp(token, "..") == 0) {
            // Вверх по иерархии
            char* last_sep = strrchr(temp_path, '\\');
            if (last_sep == nullptr) {
                last_sep = strrchr(temp_path, '/');
            }
            if (last_sep != NULL) {
                *last_sep = '\0';
            }
        }
        else {
            // Добавление компонента
            size_t len = strlen(temp_path);
            if (len > 0 && temp_path[len - 1] != '\\' && temp_path[len - 1] != '/') {
                strncat(temp_path,
#ifdef _WIN32
                    "\\",
#else
                    "/",
#endif
                    sizeof(temp_path) - len - 1);
            }
            strncat(temp_path, token, sizeof(temp_path) - strlen(temp_path) - 1);
        }
        token = strtok(NULL, "\\/");
    }

    SAFE_STRCPY(normalized_path, temp_path, normalized_path_size);
}

// Функция выбора пути к папке
void program_way() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    printf("\n");
    char folder_way_new[256] = { 0 };
    char normalized_path[256];

    printf("Введите путь к папке: ");
    getLineWithEsc("", folder_way_new, sizeof(folder_way_new));
    if (folder_way_new[0] == '\0') {
        return;
    }
    unsigned char c = folder_way_new[0];
    if ((c >= 192 && c <= 223) || (c >= 224 && c <= 255)) {
        printf("Ошибка ввода. Неверный путь к папке. ");
        printf("Используется последний корректный путь: %s\n", folder_way);
        printf("Для продолжения нажмите Enter."); system("PAUSE>nul");
        return;
    }
    normalizePath(folder_way_new, normalized_path, sizeof(normalized_path));

    struct stat program_way_buffer;
    if (stat(normalized_path, &program_way_buffer) == 0) {
        char test_file_path[512];
        snprintf(test_file_path, sizeof(test_file_path), "%s\\test_access.txt", normalized_path);
        FILE* test_file = fopen(test_file_path, "w");

        if (test_file) {
            fclose(test_file);
            remove(test_file_path);
            printf("Путь к папке выбран.\n");
            snprintf(folder_way, sizeof(folder_way), "%s\\", normalized_path); // Завершающий слеш НУЖЕН
        }
        else {
            printf("Ошибка: Нет прав для записи в указанную папку.\n");
            if (folder_way[0] != '\0') {
                printf("Используется последний корректный путь: %s\n", folder_way);
            }
        }
    }
    else {
        printf("Ошибка ввода. Неверный путь к папке.\n");
        if (folder_way[0] != '\0') {
            printf("Используется последний корректный путь: %s\n", folder_way);
        }
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
            snprintf(folder_way, sizeof(folder_way), "%s\\", current_path);
        }
        else
        {
            fprintf(stderr, "Ошибка при получении текущего пути.\n"); // обработка ошибки
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
                break;
            }
            else {
                printf("Сначала выберите путь к папке. ");
                printf("Для продолжения нажмите Enter."); system("PAUSE>nul");
                break;
            }

        case '2':
            program_way();
            break;
        case '3':
            instruction();
            break;
        case 27:
            return;
        default:
            break;
        }
    }
}