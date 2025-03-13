#include "Lab_prog_header_1.h"
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

#include <filesystem>

using namespace std;

// ���������� ���������� (�����������)
char folder_way[256] = { 0 };       // ���� � �����, �������������� ������ �������
const char* file_extension = ".txt"; // ���������� �����
const char* ocfe = "IC_";           // ������� ��� ������ ��������� ���������������
const char* oa = "AO_";             // ������� ��� ������ ������� �����������

/** @brief ��������� ������ ������ �� ������� � ���������� Esc � Backspace.
 *  @param instruction ���������, ������������ ������������ ����� ������.
 *  @return ��������� ������, ��� ������ ������, ���� ��� ����� Esc.
 */
std::string getLineWithEsc(const std::string& instruction) {
    std::string input;
    std::cout << instruction;
    while (true) {
        int key = _getch();
        if (key == 27) { // ESC
            std::cin.clear();
            input = ""; // ������� ������ �����
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

/** @brief ����������, �������� �� ��� ����������. */
bool is_leap(int year) {
    return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
}

/** @brief ��������� ������������ ����. */
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

/** @brief ��������� ������������ ����� �����. */
bool isValidFileName(const std::string& fileName) {
    // ����������� ������� � Windows
    const std::string invalidChars = "\\/:*?\"<>|";
    return fileName.find_first_of(invalidChars) == std::string::npos;
}

/** @brief ������ ���������� �� �����. */
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
        std::cout << "�� ������� ������� ���� instruction.txt" << std::endl;
        // ��������� ���������� � ���� � ����� ����������
        std::cerr << "����������, ��������� ���� instructions.txt � ��������� �����:\n";
        std::cerr << std::filesystem::current_path().string() << std::endl;

    }
    std::cout << "\n������� ����� ������� ��� �����������...\n";
    _getch();
    system("mode con cols=120 lines=30");
}

// ����� ������� ��� �������� � ������� "��� ������"
bool checkAndRequestEmpty(char field[], int& i, int current_col, const char* headers) {
    field[i] = '\0'; // ��������� ������ �������� ����

    // ����� ���������, ������ �� ����
    if (i == 0) {
        printf("\n�� �������� ���� ������, �������� '��� ������'?(y/n): ");
        while (true)
        {
            char confirm = _getch();
            if (tolower(confirm) == 'y') {
                strcpy(field, "��� ������");
                return true; // "��� ������" ���������
            }
            else if (tolower(confirm) == 'n') {
                // ������ ����� ��� �����.  ������ - ������� � �����.
                printf("\n��������� ����.\n"); // ���������
                memset(field, 0, sizeof(field));  // ������� ����
                i = 0; // ���������� ������
                return false;  //  �� �������, � ���������� ����
            }
        }
    }
    // �������� �� �������� ���� ��� ����.
    if (current_col == 1 && strcmp(headers, "��� ��������������� | ���� | �������� �����������\n") == 0) {
        if (!isValidDate(field)) {  //  �������� ����
            printf("\n�������� ������ ����. ����������, ������� ���� � ������� ��.��.����.\n��������� ����.\n");
            memset(field, 0, sizeof(field));
            i = 0;
            return false;
        }
    }

    // �������� �� �������� ���� ��� ���.
    if (current_col == 2 && strcmp(headers, "�������� ����������� | ����� | ��� ������������\n") == 0) {
        if (strlen(field) > 0) { //  �������� ���
            for (int j = 0; j < strlen(field); ++j) {
                bool valid_char = false;
                if ((field[j] >= '�' && field[j] <= '�') || (field[j] >= '�' && field[j] <= '�') ||
                    (field[j] >= 'a' && field[j] <= 'z') || (field[j] >= 'A' && field[j] <= 'Z') ||
                    field[j] == ' ' || field[j] == '-' || field[j] == '\x2D') {
                    valid_char = true;
                }
                if (!valid_char) {
                    printf("\n������������ ���� ���. ������� ��� ���: \n");
                    memset(field, 0, sizeof(field));
                    i = 0;
                    return false;
                }
            }
        }
    }

    return true; // ���� �� ������ (��� ��������� ��������� "��� ������")
}


/** @brief ����� ������� ��� �������� ����� ��� �������� */
void openFileForAppend(FILE*& file, const char* full_name, char text[][1000], const char* headers, string& mode) {
    // ����������� ������ *��* �������� �����
    if (std::filesystem::exists(full_name)) {
        mode = "�������� ����� (��������)";
    }
    else {
        mode = "�������� �����";
    }

    file = fopen(full_name, "a");  //��������� ����
    if (!file) {
        printf("������ �������� ����� ��� ��������.\n");
        printf("��� ����������� ������� Enter."); system("PAUSE>nul");
        return;
    }

    system("cls");
    printf("���� ������ ��� ��������.\n\n");
    //cout << "�����: " << mode << endl; // ������ ����� ������
    cout << "���� � �����: " << full_name << endl << endl;

    printf("������� TAB ��� �������� � ���������� ���������.\n");
    printf("������� Enter ��� ������������� �������� � ��������� ������.\n");
    printf("�������� �������� ��� ��� ������� TAB ��� ��������� ����� ��������� ��� �� �������� � ���������� ���������!\n");

    int i = 0;
    int current_col = 0;

    while (1) {
        if (current_col == 0) {
            if (strcmp(headers, "��� ��������������� | ���� | �������� �����������\n") == 0)
            {
                printf("\n��� ���������������: ");
            }
            else
            {
                printf("\n�������� �����������: ");
            }
        }
        else if (current_col == 1) {
            if (strcmp(headers, "��� ��������������� | ���� | �������� �����������\n") == 0)
            {
                printf("\n���� (��.��.����): ");
            }
            else
            {
                printf("\n�����: ");
            }
        }
        else if (current_col == 2) {
            if (strcmp(headers, "��� ��������������� | ���� | �������� �����������\n") == 0)
            {
                printf("\n�������� �����������: ");
            }
            else
            {
                printf("\n��� ������������: ");
            }
        }

        i = 0;
        while (1) {
            char c = _getch();

            if (c == 27) {
                if (file) fclose(file);
                return; // ����� �� ������� ��������
            }
            else if (c == '\t') {
                if (!checkAndRequestEmpty(text[current_col], i, current_col, headers))
                {
                    if (i == 0 && c == 27)
                    {
                        if (file) fclose(file);
                        return;
                    }
                    //��� ������� �� n, ������ �������� ������ ������
                    continue;

                }

                current_col++;
                if (current_col == 3) {
                    // �������� �� ��� ������ ���� *�������* ������
                    fprintf(file, "%s\t%s\t%s\n", text[0], text[1], text[2]); // ������
                    current_col = 0;
                    memset(text, 0, sizeof(text));
                    printf("\n");
                    break;
                }
                break;

            }
            else if (c == '\r') {
                if (!checkAndRequestEmpty(text[current_col], i, current_col, headers))
                {
                    if (i == 0 && c == 27)
                    {
                        if (file) fclose(file);
                        return;
                    }
                    //��� ������� �� n, ������ �������� ������ ������
                    continue;
                }

                // �������� *���������* ����� � ������ "��� ������" - ��������

                // *�����* �����, ������������ ��������� ����:
                bool allEmpty = true;
                for (int j = 0; j < 3; ++j) {
                    if (strlen(text[j]) > 0 && strcmp(text[j], "��� ������") != 0) {
                        allEmpty = false;
                        break;
                    }
                }

                if (allEmpty) {
                    printf("\n��� ���� ������ ��� �������� '��� ������'. ������ �� ����� ���������.\n");
                    printf("��� ����������� ������� Enter."); system("PAUSE>nul");
                }
                else {
                    // ����������� ��������: ��� �� ���� "��� ������"?
                    bool allNoData = true;
                    for (int j = 0; j < 3; j++) {
                        if (strcmp(text[j], "��� ������") != 0) {
                            allNoData = false; // ���� ���� ���� ���� �� "��� ������", �� ���������� ����
                            break;
                        }
                    }
                    if (!allNoData) //������� ��������
                    {
                        fprintf(file, "%s\t%s\t%s\n", text[0], text[1], text[2]); // ������
                    }
                    else {
                        printf("\n��� ���� ������ ��� �������� '��� ������'. ������ �� ����� ���������.\n");
                        printf("��� ����������� ������� Enter."); system("PAUSE>nul");
                    }

                }

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
                // ��������������� ���� ��������
                if (current_col == 1 && strcmp(headers, "��� ��������������� | ���� | �������� �����������\n") == 0) {
                    // ���� ����: ������ ����� � �����
                    if (isdigit(c) || c == '.') {
                        if (i < 1000 - 1) {
                            text[current_col][i++] = c;
                            printf("%c", c);
                        }
                    }
                }
                else if (current_col == 2 && strcmp(headers, "�������� ����������� | ����� | ��� ������������\n") == 0) {
                    // ���� ���: �����, ������, �����
                    if ((c >= '�' && c <= '�') || (c >= '�' && c <= '�') ||
                        (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
                        c == ' ' || c == '-') {
                        if (i < 1000 - 1) {
                            text[current_col][i++] = c;
                            printf("%c", c);
                        }
                    }
                }
                else {
                    // ��� ��������� ����� (��� ����., �������� ���., �����) - ����� ����
                    if (i < 1000 - 1) {
                        text[current_col][i++] = c;
                        printf("%c", c);
                    }
                }
            }
        }
    }
}

/** @brief ������ � ��������� ����������������. */
void outgoing_correspondence() {
    char txt_name[256];
    char text[3][1000];
    char full_name[512];
    string mode = ""; //��� �������� ������ � ������

    do {
        system("cls");
        SetConsoleCP(1251);
        SetConsoleOutputCP(1251);
        std::locale loc("ru_RU.UTF-8");
        printf("��������� ���������������\n\n");
        printf("1)   �������/������� ���� ��� ��������\n");
        printf("2)   ������������ ����\n");
        printf("Esc) ����� � ����� ���� �����\n\n");
        printf("_____________________________________________\n\n");
        char menu_out_case = _getch();
        if (menu_out_case == 27) {
            return;

        }
        FILE* file{};
        switch (menu_out_case) {
        case '1': {
            printf("\n");
            printf("������� ��� �����: ");
            string fileNameInput = getLineWithEsc("");

            if (fileNameInput.empty()) {
                break;
            }
            strcpy(txt_name, fileNameInput.c_str());
            if (!isValidFileName(txt_name)) {
                printf("������������ ������� � ����� �����.  ����������, ����������� ������ ���.\n");
                printf("��� ����������� ������� Enter."); system("PAUSE>nul");
                break;
            }

            sprintf(full_name, "%s%s%s%s", folder_way, ocfe, txt_name, file_extension);
            openFileForAppend(file, full_name, text, "��� ��������������� | ���� | �������� �����������\n", mode);
            break;
        }

        case '2': {
            printf("\n");
            printf("������� ��� ����� ��� ����������: ");
            string fileNameInput = getLineWithEsc("");

            if (fileNameInput.empty()) {
                break;
            }
            strcpy(txt_name, fileNameInput.c_str());

            if (!isValidFileName(txt_name)) {
                printf("������������ ������� � ����� �����. ����������, ����������� ������ ���.\n");
                printf("��� ����������� ������� Enter."); system("PAUSE>nul");
                break;
            }
            sprintf(full_name, "%s%s%s%s", folder_way, ocfe, txt_name, file_extension);

            if (std::filesystem::exists(full_name)) {
                printf("���� � ����� ������ ��� ����������.  ������������? (y/n): ");
                char overwriteConfirm = _getch();
                if (tolower(overwriteConfirm) != 'y') {
                    printf("\n���� �� ��� �����������.\n");
                    printf("��� ����������� ������� Enter."); system("PAUSE>nul");
                    break;
                }
                printf("\n");
                mode = "���������� �����";
            }
            else {
                mode = "�������� �����";
            }

            file = fopen(full_name, "w");
            if (file) {
                fprintf(file, "��� ��������������� | ���� | �������� �����������\n");
                fclose(file);
                printf("���� ������� �����������.\n");

                openFileForAppend(file, full_name, text, "��� ��������������� | ���� | �������� �����������\n", mode);
            }
            else {
                printf("������ ���������� �����.\n");
                printf("��� ����������� ������� Enter."); system("PAUSE>nul");
            }
            break;
        }
        default:
            printf("������������ ����. ����������, �������� 1, 2 ��� Esc.\n");
            printf("��� ����������� ������� Enter."); system("PAUSE>nul");
            break;
        }
    } while (true);
}


/** @brief ������ � �������� �����������. */
void organization_addresses() {
    char txt_name[256];
    char text[3][1000];
    char full_name[512];
    string mode = "";
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::locale loc("ru_RU.UTF-8");

    do {
        system("cls");
        printf("������ �����������\n\n");
        printf("1)   �������/������� ���� ��� ��������\n");
        printf("2)   ������������ ����\n");
        printf("Esc) ����� � ����� ���� �����\n\n");
        printf("_____________________________________________\n\n");
        char menu_out_case = _getch();
        if (menu_out_case == 27) {
            return;
        }
        FILE* file{};
        switch (menu_out_case) {
        case '1': {
            printf("\n");
            printf("������� ��� �����: ");
            string fileNameInput = getLineWithEsc("");
            if (fileNameInput.empty()) {
                break;
            }
            strcpy(txt_name, fileNameInput.c_str());

            if (!isValidFileName(txt_name)) {
                printf("������������ ������� � ����� �����. ����������, ����������� ������ ���.\n");
                printf("��� ����������� ������� Enter."); system("PAUSE>nul");
                break;
            }

            sprintf(full_name, "%s%s%s%s", folder_way, oa, txt_name, file_extension);
            openFileForAppend(file, full_name, text, "�������� ����������� | ����� | ��� ������������\n", mode);
            break;
        }

        case '2': {
            printf("\n");
            printf("������� ��� ����� ��� ����������: ");
            string fileNameInput = getLineWithEsc("");

            if (fileNameInput.empty()) {
                break;
            }
            strcpy(txt_name, fileNameInput.c_str());

            if (!isValidFileName(txt_name)) {
                printf("������������ ������� � ����� �����.  ����������, ����������� ������ ���.\n");
                printf("��� ����������� ������� Enter."); system("PAUSE>nul");
                break;
            }
            sprintf(full_name, "%s%s%s%s", folder_way, oa, txt_name, file_extension);

            if (std::filesystem::exists(full_name)) {
                printf("���� � ����� ������ ��� ����������. ������������? (y/n): ");
                char overwriteConfirm = _getch();
                if (tolower(overwriteConfirm) != 'y') {
                    printf("\n���� �� ��� �����������.\n");
                    printf("��� ����������� ������� Enter."); system("PAUSE>nul");
                    break;
                }
                printf("\n");
                mode = "���������� �����";
            }
            else {
                mode = "�������� �����";
            }

            file = fopen(full_name, "w");
            if (file) {
                fprintf(file, "�������� ����������� | ����� | ��� ������������\n");
                fclose(file);
                printf("���� ������� �����������.\n");

                openFileForAppend(file, full_name, text, "�������� ����������� | ����� | ��� ������������\n", mode);
            }
            else {
                printf("������ ���������� �����.\n");
                printf("��� ����������� ������� Enter."); system("PAUSE>nul");
            }
            break;
        }
        default:
            printf("������������ ����. ����������, �������� 1, 2 ��� Esc.\n");
            printf("��� ����������� ������� Enter."); system("PAUSE>nul");
            break;
        }
    } while (true);
}

/** @brief ������� ������ ���� ������. */
void menu_choises() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    do {
        system("cls");
        printf("���� ������ ���� ������\n\n");
        printf("1)   ��������� ���������������\n");
        printf("2)   ������ �����������\n");
        printf("Esc) ����� � ����\n");
        printf("_____________________________________________\n\n");
        printf("��������� ���� � �����: %s\n", folder_way);

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
            printf("������������ ����. ���������� �������� 1, 2, ��� Esc.\n");
            printf("��� ����������� ������� Enter."); system("PAUSE>nul");
            break;

        }
    } while (true);
}

/** @brief ����������� ���� � �����, ����������� ������������� ����.
 *  @param path ������� ����.
 *  @return ��������������� ���������� ����.
 */
std::string normalizePath(const std::string& path) {
    namespace fs = std::filesystem;
    fs::path normalizedPath = fs::absolute(path);
    return normalizedPath.string();
}

/** @brief ����� ���� � �����. */
void program_way() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    printf("\n");
    char folder_way_new[256] = { 0 };

    printf("������� ���� � �����: ");
    string pathInput = getLineWithEsc("");  // ���������� getLineWithEsc
    if (pathInput.empty()) {  // ���������, �� ��� �� ����� Esc
        return;
    }

    std::string normalized_path = normalizePath(pathInput); //��������� ������������ ����

    struct stat program_way_buffer;
    if (stat(normalized_path.c_str(), &program_way_buffer) == 0) {
        printf("���� � ����� ������. ");
        strcpy(folder_way, (normalized_path + "\\").c_str()); //��������� � �����������
    }
    else {
        printf("������ �����. �������� ���� � �����. ");
        memset(folder_way, 0, sizeof(folder_way));
    }
    printf("��� ����������� ������� Enter."); system("PAUSE>nul");
}


/** @brief ������� ����. */
void menu() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // �������������� ����������� ���� ��� �������
    if (folder_way[0] == '\0') {
        strcpy(folder_way, (normalizePath(std::filesystem::current_path().string()) + "\\").c_str());
    }

    cout << "\n��������� ��� ������� ���� ������ � ��������������� � ������� �����������.\n";

    while (true) {
        system("cls");
        printf("������� ����:\n");
        printf("1. ������ ������\n");
        printf("2. �������� ���� � �����\n");
        printf("3. ����������\n\n");
        printf("������� ����: %s\n\n", folder_way);
        printf("Esc - �����\n\n");

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
                cout << "������� �������� ���� � �����. ";
                cout << "��� ����������� ������� Enter.";
                system("PAUSE>nul");
                break;
            }

        case '2':
            program_way();
            break;
        case '3':
            instruction();
            break;
        case 27: // ����� �� ��������� �� ������� Esc � ������� ����
            return;
        default:
            cerr << "�������� �����.\n"; //������� ����� ������
            cout << "������� ����� ������� ��� �����������...\n";
            _getch();
        }
    }
}