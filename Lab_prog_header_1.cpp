#include "Lab_prog_header_1.h"
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

#include <filesystem>

using namespace std;

// Глобальные переменные (определения)
char folder_way[256] = { 0 };       // Путь к папке, инициализируем пустой строкой
const char* file_extension = ".txt"; // Расширение файла
const char* ocfe = "IC_";           // Префикс для файлов исходящей корреспонденции
const char* oa = "AO_";             // Префикс для файлов адресов организаций

/** @brief Выполняет чтение строки из консоли с поддержкой Esc и Backspace.
 *  @param instruction Сообщение, отображаемое пользователю перед вводом.
 *  @return Введенная строка, или пустая строка, если был нажат Esc.
 */
std::string getLineWithEsc(const std::string& instruction) {
    std::string input;
    std::cout << instruction;
    while (true) {
        int key = _getch();
        if (key == 27) { // ESC
            std::cin.clear();
            input = ""; // Очистка строки ввода
            std::cout << std::endl;
            return input;
        }
        else if (key == '\r') { // Enter
            std::cout << std::endl;
            return input;
        }
        else if (key == 8) { // Backspace
            if (!input.empty()) {
                input.pop_back();
                std::cout << "\b \b";
            }
        }
        else if (key >= 32 && key <= 126) { // Printable characters
            input += (char)key;
            std::cout << (char)key;
        }
    }
}

/** @brief Определяет, является ли год високосным. */
bool is_leap(int year) {
    return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
}

/** @brief Проверяет корректность даты. */
bool isValidDate(const char* dateStr) {
    if (strcmp(dateStr, "нет данных") == 0) {
        return true;
    }
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
/** @brief Проверяет корректность имени файла. */
bool isValidFileName(const std::string& fileName) {
    // Запрещенные символы в Windows
    const std::string invalidChars = "\\/:*?\"<>|";
    return fileName.find_first_of(invalidChars) == std::string::npos;
}

/** @brief Читает инструкции из файла. */
void instruction() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    system("cls");
    system("mode con cols=150 lines=36");
    std::ifstream file("instructions.txt");
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
        // Добавляем информацию о пути к файлу инструкции
        std::cerr << "Пожалуйста, поместите файл instruction.txt в следующую папку:\n";
        std::cerr << std::filesystem::current_path().string() << std::endl;

    }
    std::cout << "\nНажмите любую клавишу для продолжения...\n";
    _getch();
    system("mode con cols=120 lines=30");
}

/** @brief Общая функция для открытия файла для дозаписи */
void openFileForAppend(FILE*& file, const char* full_name, const char* headers, std::string& mode) {
    // Определение режима *ДО* открытия файла
    if (std::filesystem::exists(full_name)) {
        mode = "Открытие файла (дозапись)";
    }
    else {
        mode = "Создание файла";
    }

    file = fopen(full_name, "a");
    if (!file) {
        printf("Ошибка открытия файла для дозаписи.\n");
        printf("Для продолжения нажмите Enter."); system("PAUSE>nul");
        return;
    }

    system("cls");
    printf("Файл открыт для дозаписи.\n\n");
    cout << "Путь к файлу: " << full_name << endl << endl;
    printf("Нажмите TAB для перехода к следующему параметру.\n");
    printf("Нажмите Enter для перехода к следующему параметру.\n");
    printf("Вы можете ввести 'нет данных' в любое поле.\n"); // Добавлено сообщение

    char field[1000];
    int i = 0;

    while (true) { // Добавлен внешний цикл для непрерывной дозаписи
        for (int current_col = 0; current_col < 3; current_col++) {
            i = 0;
            memset(field, 0, sizeof(field));

            // Вывод приглашения к вводу
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

            // Цикл ввода для одного поля
            while (true) {
                char c = _getch();

                if (c == 27) {
                    if (file) fclose(file);
                    return;
                }
                else if (c == '\t' || c == '\r') {
                    field[i] = '\0'; // Завершаем строку

                    //  проверка формата даты ПОСЛЕ ввода
                    if (current_col == 1 && strcmp(headers, "Тип корреспонденции | Дата | Название организации\n") == 0) {
                        if (!isValidDate(field)) {
                            printf("\nНеверный формат даты. Пожалуйста, введите дату в формате ДД.ММ.ГГГГ или 'нет данных': \n");
                            i = 0; // Сброс
                            memset(field, 0, sizeof(field));
                            continue; //  заново
                        }
                    }
                    if (i == 0) { // Добавлена проверка на пустое поле
                        printf("\nВы ничего не ввели. Повторите ввод.");
                        printf("\nЕсли у вас нет данных для заполнения то введите 'нет данных'.");
                        printf("\nИначе, введите необходимую информацию: \n");
                        continue;
                    }

                    fprintf(file, "%s; ", field);
                    break;
                }
                else if (c == 8) {
                    if (i > 0) {
                        i--;
                        printf("\b \b");
                        field[i] = '\0';
                    }
                }
                else {
                    // Ввод символов (с проверками)
                    if (current_col == 1 && strcmp(headers, "Тип корреспонденции | Дата | Название организации\n") == 0)
                    {
                        // Разрешаем ввод букв (для "нет данных"), цифр, точек и пробелов.
                        if (isdigit(c) || c == '.' || c == ' ' || (c >= 'а' && c <= 'я') || (c >= 'А' && c <= 'Я')) {
                            if (i < 999) {
                                field[i++] = c;
                                printf("%c", c);
                            }
                        }
                    }
                    else if (current_col == 2 && strcmp(headers, "Название организации | Адрес | ФИО организатора\n") == 0) {
                        //  буквы, пробел, дефис
                        if ((c >= 'а' && c <= 'я') || (c >= 'А' && c <= 'Я') ||
                            (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
                            c == ' ' || c == '-') {
                            if (i < 1000 - 1) {
                                field[i++] = c;
                                printf("%c", c);
                            }
                        }
                    }
                    else {
                        // Для остальных полей - любой ввод
                        if (i < 1000 - 1) {
                            field[i++] = c;
                            printf("%c", c);
                        }
                    }
                }
            }
        }
        fprintf(file, "\n"); // Перенос строки после каждой тройки полей
        printf("\n"); // Добавлен отступ после ввода строки
    }
}


/** @brief Работа с исходящей корреспонденцией. */
void outgoing_correspondence() {
    char txt_name[256];
    char full_name[512];
    string mode = "";

    do {
        system("cls");
        SetConsoleCP(1251);
        SetConsoleOutputCP(1251);
        std::locale loc("ru_RU.UTF-8");
        printf("\nИсходящая корреспонденция:\n");
        printf("1.   Создать/открыть файл для дозаписи\n");
        printf("2.   Перезаписать файл\n");
        printf("Esc. Выход в выбор типа файла\n\n");
        printf("_____________________________________________\n\n");
        char menu_out_case = _getch();
        if (menu_out_case == 27) {
            return;

        }
        FILE* file{};
        switch (menu_out_case) {
        case '1': {
            printf("\n");
            printf("Введите имя файла: ");
            string fileNameInput = getLineWithEsc("");

            if (fileNameInput.empty()) {
                break;
            }
            strcpy(txt_name, fileNameInput.c_str());
            if (!isValidFileName(txt_name)) {
                printf("Недопустимые символы в имени файла.  Пожалуйста, используйте другое имя.\n");
                printf("Для продолжения нажмите Enter."); system("PAUSE>nul");
                break;
            }

            sprintf(full_name, "%s%s%s%s", folder_way, ocfe, txt_name, file_extension);
            openFileForAppend(file, full_name, "Тип корреспонденции | Дата | Название организации\n", mode);
            break;
        }

        case '2': {
            printf("\n");
            printf("Введите имя файла для перезаписи: ");
            string fileNameInput = getLineWithEsc("");

            if (fileNameInput.empty()) {
                break;
            }
            strcpy(txt_name, fileNameInput.c_str());

            if (!isValidFileName(txt_name)) {
                printf("Недопустимые символы в имени файла. Пожалуйста, используйте другое имя.\n");
                printf("Для продолжения нажмите Enter."); system("PAUSE>nul");
                break;
            }
            sprintf(full_name, "%s%s%s%s", folder_way, ocfe, txt_name, file_extension);

            if (std::filesystem::exists(full_name)) {
                printf("Файл с таким именем уже существует.  Перезаписать? (y/n): ");
                char overwriteConfirm = _getch();
                if (tolower(overwriteConfirm) != 'y') {
                    printf("\nФайл не был перезаписан.\n");
                    printf("Для продолжения нажмите Enter."); system("PAUSE>nul");
                    break;
                }
                printf("\n");
                mode = "Перезапись файла";
            }
            else {
                mode = "Создание файла";
            }

            file = fopen(full_name, "w");
            if (file) {
                fclose(file);
                printf("Файл успешно перезаписан.\n");

                openFileForAppend(file, full_name, "Тип корреспонденции | Дата | Название организации\n", mode);
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


/** @brief Работа с адресами организаций. */
void organization_addresses() {
    char txt_name[256];
    char full_name[512];
    string mode = "";
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::locale loc("ru_RU.UTF-8");

    do {
        system("cls");
        printf("\nАдреса организаций:\n");
        printf("1.   Создать/открыть файл для дозаписи\n");
        printf("2.   Перезаписать файл\n");
        printf("Esc. Выход в выбор типа файла\n\n");
        printf("_____________________________________________\n\n");
        printf("Текущий путь: %s\n\n", folder_way);
        char menu_out_case = _getch();
        if (menu_out_case == 27) {
            return;
        }
        FILE* file{};
        switch (menu_out_case) {
        case '1': {
            printf("\n");
            printf("Введите имя файла: ");
            string fileNameInput = getLineWithEsc("");
            if (fileNameInput.empty()) {
                break;
            }
            strcpy(txt_name, fileNameInput.c_str());

            if (!isValidFileName(txt_name)) {
                printf("Недопустимые символы в имени файла. Пожалуйста, используйте другое имя.\n");
                printf("Для продолжения нажмите Enter."); system("PAUSE>nul");
                break;
            }

            sprintf(full_name, "%s%s%s%s", folder_way, oa, txt_name, file_extension);
            openFileForAppend(file, full_name, "Название организации | Адрес | ФИО организатора\n", mode);
            break;
        }

        case '2': {
            printf("\n");
            printf("Введите имя файла для перезаписи: ");
            string fileNameInput = getLineWithEsc("");

            if (fileNameInput.empty()) {
                break;
            }
            strcpy(txt_name, fileNameInput.c_str());

            if (!isValidFileName(txt_name)) {
                printf("Недопустимые символы в имени файла.  Пожалуйста, используйте другое имя.\n");
                printf("Для продолжения нажмите Enter."); system("PAUSE>nul");
                break;
            }
            sprintf(full_name, "%s%s%s%s", folder_way, oa, txt_name, file_extension);

            if (std::filesystem::exists(full_name)) {
                printf("Файл с таким именем уже существует. Перезаписать? (y/n): ");
                char overwriteConfirm = _getch();
                if (tolower(overwriteConfirm) != 'y') {
                    printf("\nФайл не был перезаписан.\n");
                    printf("Для продолжения нажмите Enter."); system("PAUSE>nul");
                    break;
                }
                printf("\n");
                mode = "Перезапись файла";
            }
            else {
                mode = "Создание файла";
            }

            file = fopen(full_name, "w");
            if (file) {
                fclose(file);
                printf("Файл успешно перезаписан.\n");

                openFileForAppend(file, full_name, "Название организации | Адрес | ФИО организатора\n", mode);
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

/** @brief Подменю выбора типа данных. */
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
        case '2':
            if (folder_way[0] != '\0') {
                organization_addresses();
                break;
            }
        default:
            break;
        }
    } while (true);
}

/** @brief Нормализует путь к файлу, обрабатывая относительные пути.
 *  @param path Входной путь.
 *  @return Нормализованный абсолютный путь.
 */
std::string normalizePath(const std::string& path) {
    namespace fs = std::filesystem;
    fs::path normalizedPath = fs::absolute(path);
    return normalizedPath.string();
}

/** @brief Выбор пути к папке. */
void program_way() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    printf("\n");
    char folder_way_new[256] = { 0 };

    printf("Введите путь к папке: ");
    string pathInput = getLineWithEsc("");  // Используем getLineWithEsc
    if (pathInput.empty()) {  // Проверяем, не был ли нажат Esc
        return;
    }

    std::string normalized_path = normalizePath(pathInput); //Применяем нормализацию пути

    struct stat program_way_buffer;
    if (stat(normalized_path.c_str(), &program_way_buffer) == 0) {
        printf("Путь к папке выбран. ");
        strcpy(folder_way, (normalized_path + "\\").c_str()); //Сохраняем с добавлением
    }
    else {
        printf("Ошибка ввода. Неверный путь к папке. ");
        memset(folder_way, 0, sizeof(folder_way));
    }
    printf("Для продолжения нажмите Enter."); system("PAUSE>nul");
}


/** @brief Главное меню. */
void menu() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // Автоматическое определение пути при запуске
    if (folder_way[0] == '\0') {
        strcpy(folder_way, (normalizePath(std::filesystem::current_path().string()) + "\\").c_str());
    }

    cout << "\nПрограмма для ведения базы данных о корреспонденции и адресах организаций.\n";

    while (true) {
        system("cls");
        printf("Главное меню:\n");
        printf("1.   Начать работу\n");
        printf("2.   Изменить путь к папке\n");
        printf("3.   Инструкция\n");
        printf("Esc. Выход\n");
        printf("_____________________________________________\n\n");
        printf("Текущий путь: %s\n\n", folder_way);


        char menu_out = _getch();
        switch (menu_out) {
        case '1':
            if (folder_way[0] != '\0')
            {
                menu_choises();
                break;
            }
            else
            {
                cout << "Сначала выберите путь к папке. ";
                cout << "Для продолжения нажмите Enter.";
                system("PAUSE>nul");
                break;
            }

        case '2':
            program_way();
            break;
        case '3':
            instruction();
            break;
        case 27: // Выход из программы по нажатию Esc в главном меню
            return;
        default:
            cerr << "Неверный выбор.\n"; //Добавил вывод ошибки
            cout << "Нажмите любую клавишу для продолжения...\n";
            _getch();
        }
    }
}