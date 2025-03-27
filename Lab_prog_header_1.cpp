//Lab_prog_header_1.cpp
#include "Lab_prog_header_1.h"
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

// ���������� ���������� (�����������)
char folder_way[256] = { 0 };       // ���� � �����
const char* file_extension = ".txt";
const char* ocfe = "IC_";
const char* oa = "AO_";

// ������� �������� ������� ��� ����� �����
bool isValidFileNameChar(char c) {
    unsigned char uc = (unsigned char)c; // ��� ���������� ������ � ����������
    return (uc != '\\' && uc != '/' && uc != ':' && uc != '*' && uc != '?' && uc != '"' && uc != '<' && uc != '>' && uc != '|');
}

// ������� �������� ������� ��� ����
bool isValidPathChar(char c) {
    unsigned char uc = (unsigned char)c;
    return (uc != '*' && uc != '?' && uc != '"' && uc != '<' && uc != '>' && uc != '|');
}

// ������� ��� ��������� ������ � ������� � ����������� ��������
void getLineWithRestrictedChars(const char* instruction, char* buffer, int buffer_size, bool (*isValidCharFunc)(char)) {
    printf("%s", instruction);
    int i = 0;
    buffer[0] = '\0'; // ������������� ������

    while (true) {
        int key = _getch();
        if (key == 0 || key == 224) { // ����������� �������
            if (_kbhit()) { // ���������, ���� �� ������ ����
                _getch();   // ���������� ������ ����
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
                            // ��������� ����������� ������ � ������� ���������� �������
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
        else { // ������� ������
            char c = (char)key;
            // ��������� ������ � ������� ���������� �������
            if (isValidCharFunc(c)) {
                if (i < buffer_size - 1) {
                    buffer[i] = c;
                    printf("%c", c);
                    i++;
                }
            }
            // ���� ������ ����������, ������ �� ������ (�� ��������, �� ���������)
        }
    }
}

// ������� �������� ���� �� ������������
bool is_leap(int year) {
    return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
}

// ������� �������� ������������ ����
bool isValidDate(const char* dateStr) {
    if (strcmp(dateStr, "��� ������") == 0) {
        return true; // "��� ������" ������ �������
    }

    // �������� ������� ��.��.���� (����� � �����������)
    if (strlen(dateStr) != 10 || dateStr[2] != '.' || dateStr[5] != '.') {
        return false; // ������������ ������
    }

    int day, month, year;
    // ��������, ��� ��� ������� - ����� ��� �����
    for (int i = 0; dateStr[i] != '\0'; ++i) {
        if (!isdigit(dateStr[i]) && dateStr[i] != '.') {
            return false; // ������������ ������
        }
    }

    // �������� ��������� ����
    int count = sscanf(dateStr, "%d.%d.%d", &day, &month, &year);
    if (count != 3) {
        return false; // ������������ ������
    }

    // �������� ������������ �������� ���, ������, ����
    if (year < 1 || year > 9999 || month < 1 || month > 12 || day < 1) {
        return false; // ������������ ����
    }

    int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (is_leap(year)) {
        daysInMonth[2] = 29;
    }

    if (day > daysInMonth[month]) {
        return false; // ������������ ���� (����)
    }

    return true; // ���� ���������
}

// ������� ������ ����������
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
        printf("�� ������� ������� ���� � �����������. ���������, ��� ���� 'instruction.txt' ��������� � ��� �� �����, ��� � ���������.\n");
        char current_path[256];
        if (_getcwd(current_path, sizeof(current_path)) != NULL) {
            printf("������� �����: %s\n", current_path);
        }
        else
        {
            printf("�� ������� ���������� ������� �����.\n");
        }
    }
    printf("\n������� ����� ������� ��� �����������...\n");
    _getch();
    system("cls");
}

// ������� �������� ����� ��� �������� (��� ��������, ���� �� ����������)
// ������ ��������� mode, �.�. ����� ������ "a" (append)
void openFileForAppend(FILE*& file, const char* full_name, const char* headers) {

    file = fopen(full_name, "a");
    if (!file) {
        printf("�� ������� ������� ���� ��� ������. ��������, ���� ����� ������ ����������, ��� � ��� ��� ���� �������.\n");
        printf("��� ����������� ������� Enter.");
        system("PAUSE>nul");
        return;
    }

    system("cls");
    if (ftell(file) == 0) { // ���������, ������ �� ���� (��� ������ ��� ������)
        printf("���� '%s' ������.\n\n", full_name);
        //fprintf(file, "%s", headers); // ���������� ���������, ���� ���� �����
        fflush(file);
    }
    else {
        printf("���� '%s' ������ ��� ��������.\n\n", full_name);
    }

    printf("���� � �����: %s\n\n", full_name);
    printf("������� TAB ��� Enter ��� �������� � ���������� ���������.\n");
    printf("������� ESC ��� ������ �� ������ �����.\n");
    printf("�� ������ ������ '��� ������' � ����� ���� (����� ���� - ��� ������ '��� ������').\n");


    // ������ 256 ������ ��� ���������� ����� ������������ ������������ ������� ������
    // � ��������� ������, ����������� � �������� ������ MAX_PATH � Windows.
    char fields[3][256];
    int current_col = 0;

    while (true) {
        for (current_col = 0; current_col < 3; current_col++) {
            int i = 0;
            memset(fields[current_col], 0, sizeof(fields[current_col]));

            // ����� �������� �������� ����
            if (current_col == 0) {
                if (strcmp(headers, "��� ���������������; ���� ����������; �������� �����������;\n") == 0) printf("\n��� ���������������: "); else printf("\n�������� �����������: ");
            }
            else if (current_col == 1) {
                if (strcmp(headers, "��� ���������������; ���� ����������; �������� �����������;\n") == 0) printf("\n���� ���������� (��.��.���� ��� ��� ������): "); else printf("\n�����: ");
            }
            else if (current_col == 2) {
                if (strcmp(headers, "��� ���������������; ���� ����������; �������� �����������;\n") == 0) printf("\n�������� �����������: "); else printf("\n������� ������������: ");
            }

            // ���� ����� ��� �������� ���� � ��������� ��������
            while (true) {
                int key = _getch();
                char c = (char)key;
                bool allowed = false;
                if (key == 0 || key == 224) { // ����������� �������
                    if (_kbhit()) { // ���������, ���� �� ������ ����
                        _getch();   // ���������� ������ ����
                        continue;
                    }
                }
                // ����������, �������� �� ������ ��� �������� ����
                unsigned char uc = (unsigned char)c; // ��� ���������� ������ � ����������
                bool is_letter = (uc >= 'A' && uc <= 'Z') || (uc >= 'a' && uc <= 'z') || (uc >= 192); // �������� + ��������� (�����)
                bool is_digit = (uc >= '0' && uc <= '9');
                bool is_space = (uc == ' ');

                if (current_col == 0) { // ��� ��������������� / �������� ����������� (������ ����)
                    if (strcmp(headers, "��� ���������������; ���� ����������; �������� �����������;\n") == 0) { // ��� ���������������
                        allowed = is_letter || is_digit || is_space || uc == '-' || uc == '_' || uc == '.' || uc == ',' || uc == '(' || uc == ')';
                    }
                    else { // �������� ����������� (�� �������)
                        allowed = is_letter || is_digit || is_space || uc == '-' || uc == '_' || uc == '.' || uc == ',' || uc == '"' || uc == '(' || uc == ')' || uc == 211 || uc == '+' || uc == '!' || uc == '&' || uc == ':' || uc == 171 || uc == 187 || uc == '#'; // �(211), �(171), �(187)
                    }
                }
                else if (current_col == 1) { // ���� ���������� / �����
                    if (strcmp(headers, "��� ���������������; ���� ����������; �������� �����������;\n") == 0) { // ���� ����������
                        allowed = is_digit || uc == '.' || is_letter || is_space; // ��������� ����� � ������ ��� "��� ������"
                    }
                    else { // �����
                        allowed = is_letter || is_digit || is_space || uc == '-' || uc == ',' || uc == '.' || uc == '/' || uc == 211; // �(211)
                    }
                }
                else if (current_col == 2) { // �������� ����������� / ������� ������������
                    if (strcmp(headers, "��� ���������������; ���� ����������; �������� �����������;\n") == 0) { // �������� ����������� (�� ���������������)
                        allowed = is_letter || is_digit || is_space || uc == '-' || uc == '_' || uc == '.' || uc == ',' || uc == '"' || uc == '(' || uc == ')' || uc == 211 || uc == '+' || uc == '!' || uc == '&' || uc == ':' || uc == 171 || uc == 187 || uc == '#'; // �(211), �(171), �(187)
                    }
                    else { // ������� ������������
                        allowed = is_letter || uc == '-' || is_space;
                    }
                }

                // ��������� ������ ����������
                if (key == 27) { // ESC
                    if (file) fclose(file);
                    printf("\n���� �������.\n");
                    return;
                }
                else if (key == '\t' || key == '\r') { // Tab or Enter
                    fields[current_col][i] = '\0';

                    // �������� ���� ����� �����
                    if (current_col == 1 && strcmp(headers, "��� ���������������; ���� ����������; �������� �����������;\n") == 0) {
                        if (!isValidDate(fields[current_col])) {
                            // ���������� ��� ������ ����
                            bool format_error = true;
                            if (strlen(fields[current_col]) == 10 && fields[current_col][2] == '.' && fields[current_col][5] == '.') {
                                int d, m, y;
                                if (sscanf(fields[current_col], "%d.%d.%d", &d, &m, &y) == 3) {
                                    format_error = false; // ������ ������, ������ ���� �����������
                                }
                            }
                            if (format_error) {
                                printf("\n������������ ������ ����. ������� ���� � ������� '��.��.����' (��������, 01.01.2023) ��� ������� '��� ������'.\n");
                            }
                            else {
                                printf("\n������������ ����. ���������, ��� ��������� ����, ����� � ��� ����������. ��������, 31.02.2023 - ������������ ����.\n");
                            }
                            i = 0;
                            memset(fields[current_col], 0, sizeof(fields[current_col]));
                            // ������ ������� ����������� ��� ����� ����
                            printf("\n���� ���������� (��.��.���� ��� ��� ������): ");
                            continue; // ���������� ���� ��� ����� �� ����
                        }
                    }

                    // �������� �� ������ ����
                    if (i == 0 && strcmp(fields[current_col], "��� ������") != 0 && !(current_col == 1 && strcmp(headers, "��� ���������������; ���� ����������; �������� �����������;\n") == 0 && fields[current_col][0] == '\0'))
                    {
                        printf("\n�� �� ����� ������� ������. ����������, ������� ���������� ��� ������� '��� ������'.\n");
                        // ������ ������� ����������� ��� �����
                        if (current_col == 0) { if (strcmp(headers, "��� ���������������; ���� ����������; �������� �����������;\n") == 0) printf("\n��� ���������������: "); else printf("\n�������� �����������: "); }
                        else if (current_col == 1) { if (strcmp(headers, "��� ���������������; ���� ����������; �������� �����������;\n") == 0) printf("\n���� ���������� (��.��.���� ��� ��� ������): "); else printf("\n�����: "); }
                        else if (current_col == 2) { if (strcmp(headers, "��� ���������������; ���� ����������; �������� �����������;\n") == 0) printf("\n�������� �����������: "); else printf("\n������� ������������: "); }
                        continue; // ���������� ���� ��� ����� �� ����
                    }
                    break; // ������� � ���������� ����
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
                                    // ��������� ����������� ������ �� ������������ ��� �������� ����
                                    unsigned char uct = (unsigned char)pszText[j];
                                    bool allowed_paste = false;
                                    bool is_letter_p = (uct >= 'A' && uct <= 'Z') || (uct >= 'a' && uct <= 'z') || (uct >= 192);
                                    bool is_digit_p = (uct >= '0' && uct <= '9');
                                    bool is_space_p = (uct == ' ');

                                    if (current_col == 0) { if (strcmp(headers, "��� ���������������; ���� ����������; �������� �����������;\n") == 0) { allowed_paste = is_letter_p || is_digit_p || is_space_p || uct == '-' || uct == '_' || uct == '.' || uct == ',' || uct == '(' || uct == ')'; } else { allowed_paste = is_letter_p || is_digit_p || is_space_p || uct == '-' || uct == '_' || uct == '.' || uct == ',' || uct == '(' || uct == ')' || uct == 211 || uct == '+' || uct == '!' || uct == '&' || uct == ':' || uct == 171 || uct == 187 || uct == '#'; } }
                                    else if (current_col == 1) { if (strcmp(headers, "��� ���������������; ���� ����������; �������� �����������;\n") == 0) { allowed_paste = is_digit_p || uct == '.' || is_letter_p || is_space_p; } else { allowed_paste = is_letter_p || is_digit_p || is_space_p || uct == '-' || uct == ',' || uct == '.' || uct == '/' || uct == 211; } }
                                    else if (current_col == 2) { if (strcmp(headers, "��� ���������������; ���� ����������; �������� �����������;\n") == 0) { allowed_paste = is_letter_p || is_digit_p || is_space_p || uct == '-' || uct == '_' || uct == '.' || uct == ',' || uct == '(' || uct == ')' || uct == 211 || uct == '+' || uct == '!' || uct == '&' || uct == ':' || uct == 171 || uct == 187 || uct == '#'; } else { allowed_paste = is_letter_p || uct == '-' || is_space_p; } }

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
                else { // ������� ������
                    if (allowed && i < sizeof(fields[current_col]) - 1) {
                        fields[current_col][i] = c;
                        printf("%c", c);
                        i++;
                    }
                    // ���� ������ �� �������� (allowed == false), ������ �� ������
                }
            } // ����� while(true) ��� ����� ������ ����
        } // ����� for �� ����� (current_col)

        if (strcmp(fields[0], "��� ������") == 0 &&
            strcmp(fields[1], "��� ������") == 0 &&
            strcmp(fields[2], "��� ������") == 0)
        {
            // ���� ��� ���� "��� ������", ������� ��������� � �� ����������
            printf("\n��� ���� �������� '��� ������'. ������ �� ����� ���������.\n");
        }
        else
        {
            // ���� ���� �� ���� ���� �� "��� ������", ���������� ������
            fprintf(file, "%s; %s; %s;\n", fields[0], fields[1], fields[2]);
            fflush(file); // ���������� ����� ����� �� ���� (��� ����������)
            printf("\n������ ���������. ������� ��������� ������ ��� ������� ESC ��� ������.\n");
        }
    }
}

// ������� ��� ������ � ��������� ����������������
void outgoing_correspondence() {
    char txt_name[256];
    char full_name[512];

    do {
        system("cls");
        SetConsoleCP(1251);
        SetConsoleOutputCP(1251);
        printf("��������� ���������������:\n");
        printf("1.   �������/������� ���� ��� ��������\n");
        printf("2.   ������������ ����\n");
        printf("Esc. ����� � ����� ���� �����\n\n");
        printf("_____________________________________________\n\n");
        printf("������� ����: %s\n\n", folder_way);

        char menu_out_case = _getch();
        if (menu_out_case == 27) {
            return;
        }

        FILE* file;
        switch (menu_out_case) {
        case '1': {
            printf("\n");
            // ���������� getLineWithRestrictedChars � ��������� ��� ����� �����
            getLineWithRestrictedChars("������� ��� �����: ", txt_name, sizeof(txt_name), isValidFileNameChar);
            if (txt_name[0] == '\0') { // ���� ������������ ����� Esc
                break;
            }
            // �������������� �������� �� ������ ���� ����� getLine...
            if (strlen(txt_name) == 0) {
                printf("��� ����� �� ����� ���� ������.\n");
                printf("��� ����������� ������� Enter."); system("PAUSE>nul");
                break;
            }


            snprintf(full_name, sizeof(full_name), "%s%s%s%s", folder_way, ocfe, txt_name, file_extension);
            openFileForAppend(file, full_name, "��� ���������������; ���� ����������; �������� �����������;\n");
            break;
        }
        case '2': {
            printf("\n");
            getLineWithRestrictedChars("������� ��� ����� ��� ����������: ", txt_name, sizeof(txt_name), isValidFileNameChar);
            if (txt_name[0] == '\0') { // Esc
                break;
            }
            if (strlen(txt_name) == 0) {
                printf("��� ����� �� ����� ���� ������.\n");
                printf("��� ����������� ������� Enter."); system("PAUSE>nul");
                break;
            }

            snprintf(full_name, sizeof(full_name), "%s%s%s%s", folder_way, ocfe, txt_name, file_extension);

            bool exists = (_access(full_name, 0) == 0);
            if (exists) {
                printf("���� � ����� ������ ��� ����������. ������������? (y/n): ");
                char overwriteConfirm = _getch();
                printf("\n"); // ������� ������ ����� _getch
                if (tolower(overwriteConfirm) != 'y') {
                    printf("���� �� ��� �����������.\n");
                    printf("��� ����������� ������� Enter."); system("PAUSE>nul");
                    break;
                }
            }

            file = fopen(full_name, "w"); // ��������� ��� ���������� ("w")
            if (file) {
                fclose(file); // ���������, ����� openFileForAppend ��� ������� � "a"
                printf("���� '%s' ������� %s.\n", full_name, exists ? "�����������" : "������");
                // ��������� ��� ���������� ������ (��������� ��������� �������������)
                openFileForAppend(file, full_name, "��� ���������������; ���� ����������; �������� �����������;\n");
            }
            else {
                printf("�� ������� ������� ���� ��� ������. ��������, ���� ����� ������ ����������, ��� � ��� ��� ���� �������.\n");
                printf("��� ����������� ������� Enter."); system("PAUSE>nul");
            }
            break;
        }
        default:
            // ������������ ���� � ���� �������������� ������ do-while
            break;
        }
    } while (true);
}

// ������� ��� ������ � �������� �����������
void organization_addresses() {
    char txt_name[256];
    char full_name[512];

    do {
        system("cls");
        SetConsoleCP(1251);
        SetConsoleOutputCP(1251);
        printf("������ �����������:\n");
        printf("1.   �������/������� ���� ��� ��������\n");
        printf("2.   ������������ ����\n");
        printf("Esc. ����� � ����� ���� �����\n\n");
        printf("_____________________________________________\n\n");
        printf("������� ����: %s\n\n", folder_way);
        char menu_out_case = _getch();
        if (menu_out_case == 27) {
            return;
        }

        FILE* file;
        switch (menu_out_case) {
        case '1': {
            printf("\n");
            getLineWithRestrictedChars("������� ��� �����: ", txt_name, sizeof(txt_name), isValidFileNameChar);
            if (txt_name[0] == '\0') { // Esc
                break;
            }
            if (strlen(txt_name) == 0) {
                printf("��� ����� �� ����� ���� ������.\n");
                printf("��� ����������� ������� Enter."); system("PAUSE>nul");
                break;
            }

            snprintf(full_name, sizeof(full_name), "%s%s%s%s", folder_way, oa, txt_name, file_extension);
            openFileForAppend(file, full_name, "�������� �����������; �����; ������� ������������;\n");
            break;
        }
        case '2': {
            printf("\n");
            getLineWithRestrictedChars("������� ��� ����� ��� ����������: ", txt_name, sizeof(txt_name), isValidFileNameChar);
            if (txt_name[0] == '\0') { // Esc
                break;
            }
            if (strlen(txt_name) == 0) {
                printf("��� ����� �� ����� ���� ������.\n");
                printf("��� ����������� ������� Enter."); system("PAUSE>nul");
                break;
            }

            snprintf(full_name, sizeof(full_name), "%s%s%s%s", folder_way, oa, txt_name, file_extension);

            bool exists = (_access(full_name, 0) == 0);
            if (exists) {
                printf("���� � ����� ������ ��� ����������. ������������? (y/n): ");
                char overwriteConfirm = _getch();
                printf("\n");
                if (tolower(overwriteConfirm) != 'y') {
                    printf("���� �� ��� �����������.\n");
                    printf("��� ����������� ������� Enter."); system("PAUSE>nul");
                    break;
                }
            }

            file = fopen(full_name, "w");
            if (file) {
                fclose(file);
                printf("���� '%s' ������� %s.\n", full_name, exists ? "�����������" : "������");
                openFileForAppend(file, full_name, "�������� �����������; �����; ������� ������������;\n");
            }
            else {
                printf("�� ������� ������� ���� ��� ������. ��������, ���� ����� ������ ����������, ��� � ��� ��� ���� �������.\n");
                printf("��� ����������� ������� Enter."); system("PAUSE>nul");
            }
            break;
        }
        default:
            break;
        }
    } while (true);
}

// ������� ���� ������ ���� ������
void menu_choises() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    do {
        system("cls");
        printf("���� ������ ���� ������:\n");
        printf("1.   ��������� ���������������\n");
        printf("2.   ������ �����������\n");
        printf("Esc. ����� � ����\n\n");
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
            }
            else {
                printf("������� �������� ���� � ����� � ������� ���� (����� 2).\n");
                printf("��� ����������� ������� Enter."); system("PAUSE>nul");
            }
            break;
        case '2':
            if (folder_way[0] != '\0') {
                organization_addresses();
            }
            else {
                printf("������� �������� ���� � ����� � ������� ���� (����� 2).\n");
                printf("��� ����������� ������� Enter."); system("PAUSE>nul");
            }
            break;

        default:
            break;
        }
    } while (true);
}

// ������� ������������ ���� (���������� ��� Windows)
void normalizePath(const char* path, char* normalized_path, size_t normalized_path_size) {
    if (path[0] == '\0') {
        if (_getcwd(normalized_path, normalized_path_size) == NULL) {
            fprintf(stderr, "�� ������� ���������� ������� �����.\n");
            normalized_path[0] = '\0';
        }
        return;
    }

    // ���������� _fullpath ��� ������������ ��� Windows
    if (_fullpath(normalized_path, path, normalized_path_size) == NULL) {
        // ������ ������������ (��������, ���� ������� ������� ��� ������������)
        // � normalized_path ����� ���� �����, ������� ���
        SAFE_STRCPY(normalized_path, path, normalized_path_size); // �������� ��� ���� ��� ���������� ��������
        // ����� �������� ����� ������, �� stat ����� ��� ����� ��������
    }

    // ������������� ������� ������������� �����, �.�. _fullpath �� ����� ��������
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

    // ������� ����������� ����, ���� �� �� �������� ������ ��������� ���� (C:\)
    size_t len = strlen(normalized_path);
    if (len > 3 && normalized_path[len - 1] == '\\') {
        normalized_path[len - 1] = '\0';
    }
}

// ������� ������ ���� � �����
void program_way() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    printf("\n");
    char folder_way_new[256] = { 0 };
    char normalized_path_temp[256]; // ��������� ����� ��� ������������

    // ���� ���� � ��������� ��������
    getLineWithRestrictedChars("������� ���� � �����: ", folder_way_new, sizeof(folder_way_new), isValidPathChar);

    if (folder_way_new[0] == '\0') { // ������������ ����� Esc
        return;
    }

    // ������������ ����
    normalizePath(folder_way_new, normalized_path_temp, sizeof(normalized_path_temp));

    // �������� ������������� ���� � ���� �������
    struct stat path_stat;
    if (stat(normalized_path_temp, &path_stat) == 0) { // ���� ����������
        if (path_stat.st_mode & S_IFDIR) { // ��� ����������
            // �������� ���� �� ������ (�������� ������� � ������� �������� ����)
            char test_file_path[512];
            // ��������� ���� � ���������������� ���� ��� snprintf, ���� ��� ���
            size_t len_norm = strlen(normalized_path_temp);
            char path_with_slash[257]; // +1 ��� ����� � +1 ��� ����
            SAFE_STRCPY(path_with_slash, normalized_path_temp, sizeof(path_with_slash));
            if (len_norm > 0 && path_with_slash[len_norm - 1] != '\\') {
                if (len_norm < sizeof(path_with_slash) - 1) {
                    path_with_slash[len_norm] = '\\';
                    path_with_slash[len_norm + 1] = '\0';
                }
                else {
                    // ���� ������� ������� ��� ���������� �����, �������� ������
                    printf("��������� ���� �� ����������. ��������� ������������ ��������� � ���������, ��� ����� ����������.\n");
                    if (folder_way[0] != '\0') printf("������������ ��������� ���������� ����: %s\n", folder_way);
                    printf("��� ����������� ������� Enter."); system("PAUSE>nul");
                    return;
                }
            }

            snprintf(test_file_path, sizeof(test_file_path), "%stest_access.tmp", path_with_slash); // ���������� ��������� ����������

            FILE* test_file = fopen(test_file_path, "w");
            if (test_file) { // ����� �� ������ ����
                fclose(test_file);
                remove(test_file_path); // ������� �������� ����
                printf("���� � ����� ������: %s\n", path_with_slash);
                SAFE_STRCPY(folder_way, path_with_slash, sizeof(folder_way)); // ��������� ���� �� ������
            }
            else { // ��� ���� �� ������
                printf("��� ���� ��� ������� � ��������� �����. �������� ������ ����� ��� ���������� � ��������������.\n");
                if (folder_way[0] != '\0') printf("������������ ��������� ���������� ����: %s\n", folder_way);
            }
        }
        else { // ���� ����������, �� ��� �� ����������
            printf("��������� ���� �� �������� ������.\n");
            if (folder_way[0] != '\0') printf("������������ ��������� ���������� ����: %s\n", folder_way);
        }
    }
    else { // ���� �� ����������
        printf("��������� ���� �� ����������. ��������� ������������ ��������� � ���������, ��� ����� ����������.\n");
        if (folder_way[0] != '\0') printf("������������ ��������� ���������� ����: %s\n", folder_way);
    }

    printf("��� ����������� ������� Enter."); system("PAUSE>nul");
}

// ������� ���� ���������
void menu() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // �������������� ����������� ���� ��� �������
    if (folder_way[0] == '\0') {
        char current_path[256];
        if (_getcwd(current_path, sizeof(current_path)) != NULL)
        {
            // ��������� ���� � �����
            size_t len = strlen(current_path);
            if (len > 0 && current_path[len - 1] != '\\' && len < sizeof(folder_way) - 1) {
                current_path[len] = '\\';
                current_path[len + 1] = '\0';
            }
            SAFE_STRCPY(folder_way, current_path, sizeof(folder_way));
        }
        else
        {
            fprintf(stderr, "�� ������� ���������� ������� �����.\n");
        }
    }

    printf("\n��������� ��� ������� ���� ������ � ��������������� � ������� �����������.\n");

    while (true) {
        system("cls");

        printf("*************************************************************\n");
        printf("*    ��������� ��� ������� ���� ������ � ���������������    *\n");
        printf("*                 � ������� �����������.                    *\n");
        printf("*      ��������� ���������, �������� � ������� ������.      *\n");
        printf("*************************************************************\n");

        printf("������� ����:\n");
        printf("1.   ������ ������\n");
        printf("2.   �������� ���� � �����\n");
        printf("3.   ����������\n");
        printf("Esc. �����\n");
        printf("_____________________________________________\n\n");
        printf("������� ����: %s\n\n", folder_way);


        char c = _getch();
        switch (c) {
        case '1':
            if (folder_way[0] != '\0') {
                menu_choises();
            }
            else {
                printf("������� �������� ���� � ����� (����� 2).\n");
                printf("��� ����������� ������� Enter."); system("PAUSE>nul");
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
            // ���������� �������� ���� � ����
            break;
        }
    }
}