#include "Lab_prog_header_1.h"
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

using namespace std;

// ���������� ���������� (�����������)
char folder_way[256] = { 0 }; // ���� � �����, �������������� ������ �������
char txt_name[256]; // ��� �����
char text[COL_COUNT][MAX_LENGTH]; // ����� ��� ����� ������
char full_name[512]; // ������ ���� � �����
const char* file_extension = ".txt"; // ���������� �����
const char* ocfe = "IC_"; // ������� ��� ������ ��������� ���������������
const char* oa = "AO_"; // ������� ��� ������ ������� �����������
char menu_out; // ����� ������������ � ������� ����
char menu_out_case; // ����� ������������ � �������
char sel; // ����� ������������ ������ ������

// ������� ��� ���������
void tabul(int x) {
    for (int i = x; i != 0; i--) printf("   ");
}

// ������� ��� �������� ����������� ����
bool is_leap(int year) {
    return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
}

// ������� ��� �������� ������������ ����
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

// ������� ��� �������� ������������ ����� �����
bool isValidFileName(const std::string& fileName) {
    // ����������� ������� � Windows
    const std::string invalidChars = "\\/:*?\"<>|";
    return fileName.find_first_of(invalidChars) == std::string::npos;
}

// ������� ��� ����������� ���������� �� ���������� �����
void instruction() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    system("cls");
    system("mode con cols=200 lines=45"); //��������� ������� ���� ��� ��������
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
        std::cout << "�� ������� ������� ���� instruction.txt" << std::endl;
    }
    printf("\n> ");
    system("PAUSE>nul");
    system("mode con cols=120 lines=30"); // ������������� ����������� ������ ���� ����������� ����������
}

// ������� ��� ������ � ��������� ����������������
void outgoing_correspondence() {
    do {
        system("cls");
        SetConsoleCP(1251);
        SetConsoleOutputCP(1251);
        std::locale loc("ru_RU.UTF-8");
        for (int i = 4; i != 0; i--) printf("\n");
        tabul(11); printf("��������� ���������������\n\n");
        tabul(11); printf("1)   ������� ����� ����\n");
        tabul(11); printf("2)   ������� ����\n");
        tabul(11); printf("Esc) ����� � ����� ���� �����\n\n");
        tabul(11); printf("_____________________________________________\n\n");
        tabul(11); printf("> ");
        menu_out_case = _getch();
        if (menu_out_case == 27) { return; } // ������� � ���������� ����
        FILE* file{};
        switch (menu_out_case) {
        case '1': {
            printf("\n");
            tabul(11); printf("������� ��� �����: ");
            string fileNameInput;
            scanf("%255[^\n]", txt_name);
            getchar(); // ������� �����
            if (!isValidFileName(txt_name)) {
                tabul(11); printf("������������ ������� � ����� �����. ����������, ����������� ������ ���.\n");
                printf("��� ����������� ������� ENTER."); system("PAUSE>nul");
                break;
            }


            sprintf(full_name, "%s%s%s%s", folder_way, ocfe, txt_name, file_extension);
            file = fopen(full_name, "w");
            if (file) {
                fclose(file);
                tabul(11); printf("���� ������� ������. ");
            }
            else {
                tabul(11); printf("������ ��������. ���� �� ��� ������. ");
            }
            printf("��� ����������� ������� ENTER."); system("PAUSE>nul");
            break;
        }

        case '2': {
            int i = 0;
            int current_col = 0;
            printf("\n");
            tabul(11); printf("������� ��� �����: "); scanf("%255s", txt_name);
            tabul(11); printf("�������� ����� ������ � ������: \n");
            tabul(11); printf("1)   ������ � ����\n");
            tabul(11); printf("2)   �������� � ����\n");
            tabul(11); printf("Esc) ����� � ����� �����\n\n");
            sprintf(full_name, "%s%s%s%s", folder_way, ocfe, txt_name, file_extension);
            tabul(11); printf("> ");
            sel = _getch();
            if (sel == 27) { break; }
            switch (sel) {
            case '1':
                file = fopen(full_name, "w");
                fprintf(file, "��� ��������������� | ���� | �������� �����������\n");
                break;
            case '2':
                file = fopen(full_name, "a");
                break;
            default:
                break; // default case ��� ��������� ������������� �����
            }
            if (sel != '1' && sel != '2') { break; }
            system("cls");
            printf("���� ������.\n\n");
            printf("������� TAB ��� �������� � ���������� ���������.\n");
            printf("������� Enter ��� ������������� �������� � ��������� ������.\n");
            printf("�������� �������� ��� ��� ������� TAB ��� ��������� ����� ��������� ��� �� �������� � ���������� ���������!\n");
            while (1) {

                if (current_col == 0) {
                    printf("\n��� ���������������: ");
                }
                else if (current_col == 1) {
                    printf("\n���� (��.��.����): ");
                }
                else if (current_col == 2) {
                    printf("\n�������� �����������: ");
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
                            printf("\n�������� ������ ����. ����������, ������� ���� � ������� ��.��.����: ");
                            memset(text[current_col], 0, sizeof(text[current_col]));
                            i = 0;
                            continue;
                        }

                        current_col++;
                        if (current_col == COL_COUNT) {
                            for (int j = 0; j < COL_COUNT; ++j) {
                                if (strlen(text[j]) == 0) {
                                    strcpy(text[j], "��� ������");
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
                                strcpy(text[j], "��� ������");
                            }
                        }
                        if (current_col == 1 && !isValidDate(text[1]))
                        {
                            printf("\n�������� ������ ����. ����������, ������� ���� � ������� ��.��.����\n");
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
                        if (current_col == 1) { // ������ ��� ������� � �����
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
                        else { // ��� ��������� �������
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
            break;
        }
        }
    } while (true);
}

// ������� ��� ������ � �������� �����������
void organization_addresses() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::locale loc("ru_RU.UTF-8");

    do {
        system("cls");
        for (int i = 4; i != 0; i--) printf("\n");
        tabul(10); printf("������ �����������\n\n");
        tabul(11); printf("1)   ������� ����� ����\n");
        tabul(11); printf("2)   ������� ����\n");
        tabul(11); printf("Esc) ����� � ����� ���� �����\n\n");
        tabul(11); printf("_____________________________________________\n\n");
        tabul(11); printf("> ");
        menu_out_case = _getch();
        if (menu_out_case == 27) { return; } // ������� � ���������� ����
        FILE* file{};
        switch (menu_out_case) {
        case '1': {
            printf("\n");
            tabul(11); printf("������� ��� �����: ");

            scanf("%255[^\n]", txt_name);
            getchar();
            if (!isValidFileName(txt_name)) {
                tabul(11); printf("������������ ������� � ����� �����. ����������, ����������� ������ ���.\n");
                printf("��� ����������� ������� ENTER."); system("PAUSE>nul");
                break;
            }

            sprintf(full_name, "%s%s%s%s", folder_way, oa, txt_name, file_extension);
            file = fopen(full_name, "w");
            if (file) {
                fclose(file);
                tabul(11); printf("���� ������� ������. ");
            }
            else {
                tabul(11); printf("������ ��������. ���� �� ��� ������. ");
            }
            printf("��� ����������� ������� ENTER."); system("PAUSE>nul");
            break;
        }

        case '2': {
            int i = 0;
            int current_col = 0;
            printf("\n");
            tabul(11); printf("������� ��� �����: "); scanf("%255s", txt_name);
            tabul(11); printf("�������� ����� ������ � ������: \n");
            tabul(11); printf("1) ������ � ����\n");
            tabul(11); printf("2) �������� � ����\n");
            tabul(11); printf("Esc)����� � ����� �����\n\n");
            sprintf(full_name, "%s%s%s%s", folder_way, oa, txt_name, file_extension);
            tabul(11); printf("> ");
            sel = _getch();
            if (sel == 27) { break; }
            switch (sel) {
            case '1':
                file = fopen(full_name, "w");
                fprintf(file, "�������� ����������� | ����� | ��� ������������\n");
                break;
            case '2':
                file = fopen(full_name, "a");
                break;
            default:
                break; // default case ��� ��������� ������������� �����
            }
            if (sel != '1' && sel != '2') { break; }
            system("cls");
            printf("���� ������.\n\n");
            printf("������� TAB ��� �������� � ���������� ���������.\n");
            printf("������� Enter ��� ������������� �������� � ��������� ������.\n");
            printf("�������� �������� ��� ��� ������� TAB ��� ��������� ����� ��������� ��� �� �������� � ���������� ���������!\n");
            while (1) {

                if (current_col == 0) {
                    printf("\n�������� �����������: ");
                }
                else if (current_col == 1) {
                    printf("\n�����: ");
                }
                else if (current_col == 2) {
                    printf("\n��� ������������: ");
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
                                    strcpy(text[j], "��� ������");
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
                                strcpy(text[j], "��� ������");
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
                        if (current_col == 2) { //�������� ���
                            bool valid_char = false;
                            if ((c >= '�' && c <= '�') || (c >= '�' && c <= '�') ||
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
            if (file) fclose(file);
            break;
        }
        }
    } while (true);
}

// ������� ��� ������� ������ ���� ������
void menu_choises() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    do {
        system("cls");
        for (int i = 3; i != 0; i--) printf("\n");
        tabul(11); printf("���� ������ ���� ������\n\n");
        tabul(11); printf("1)   ��������� ���������������\n");
        tabul(11); printf("2)   ������ �����������\n");
        tabul(11); printf("Esc) ����� � ����\n");
        for (int i = 2; i != 0; i--) printf("\n");
        tabul(11); printf("_____________________________________________\n\n");
        tabul(11); printf("��������� ���� � �����: %s\n", folder_way);
        tabul(11); printf("> ");
        menu_out = _getch();
        if (menu_out == 27) { return; } // ������� � ������� ����
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
    } while (true);
}

// ������� ��� ������ ���� � �����
void program_way() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    printf("\n");
    char folder_way_new[256] = { 0 };

    tabul(11); printf("������� ���� � �����: ");
    scanf("%255[^\n]", folder_way_new);
    getchar();

    size_t len = strlen(folder_way_new);
    if (len > 0 && folder_way_new[len - 1] != '\\') {
        folder_way_new[len] = '\\';
        folder_way_new[len + 1] = '\0';
    }

    struct stat program_way_buffer;
    if (stat(folder_way_new, &program_way_buffer) == 0) {
        tabul(11); printf("���� � ����� ������. ");
        strcpy(folder_way, folder_way_new);
    }
    else {
        tabul(11); printf("������ �����. �������� ���� � �����. ");
        memset(folder_way_new, 0, sizeof(folder_way_new));
        // ������� ���������� folder_way
        memset(folder_way, 0, sizeof(folder_way));
    }
    printf("��� ����������� ������� ENTER."); system("PAUSE>nul");
}

// ������� ������� ����
void menu() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    do {
        system("cls");
        for (int i = 3; i != 0; i--) printf("\n");
        tabul(11); printf("����\n\n");
        tabul(11); printf("1)   ������ ������ � �������\n");
        tabul(11); printf("2)   ���� � ����� �������� ������\n");
        tabul(11); printf("3)   ����������\n");
        tabul(11); printf("Esc) ����� �� ���������\n\n");
        tabul(11); printf("_____________________________________________\n\n");
        tabul(11); printf("��������� ���� � �����: %s\n", folder_way);
        tabul(11); printf("> ");
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
                printf("������� �������� ���� � �����. ");
                printf("��� ����������� ������� ENTER.");
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
        }
    } while (true);
}