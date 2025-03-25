// Lab_prog_header_1.cpp
#include "Lab_prog_header_1.h"
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

// ���������� ���������� (�����������)
char folder_way[256] = { 0 };
const char* file_extension = ".txt";
const char* ocfe = "IC_";
const char* oa = "AO_";

// ������ ��� ����������� ����������� �����
#define SAFE_STRCPY(dest, src, size) do { strncpy(dest, src, size - 1); dest[size - 1] = '\0'; } while(0)

// ������� ��� ��������� ������ � ������� (Ctrl+V ����� WinAPI)
void getLineWithEsc(const char* instruction, char* buffer, int buffer_size) {
    printf("%s", instruction);
    int i = 0;
    while (true) {
        int key = _getch();
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
        else if (key == 9) { // Tab
            if (i < buffer_size - 1) {
                buffer[i] = '\t';
                printf("\t");
                i++;
            }
        }
        else if (key == 22) { // Ctrl+V
            if (OpenClipboard(NULL)) {
                HANDLE hData = GetClipboardData(CF_TEXT);
                if (hData != NULL) {
                    char* pszText = (char*)GlobalLock(hData);
                    if (pszText != NULL) {
                        // �������� �� ������ ������ � ��� �����
                        for (int j = 0; pszText[j] != '\0' && i < buffer_size - 1; ++j) {
                            if (pszText[j] >= 32 && pszText[j] <= 255)
                            {
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
        else if (key == 0 || key == 224) {
            _getch();
        }
        else if (key >= 32 && key <= 255) {
            if (i < buffer_size - 1) {
                buffer[i] = (char)key;
                printf("%c", (char)key);
                i++;
            }
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
        return true;
    }
    if (strlen(dateStr) != 10 && strcmp(dateStr, "��� ������") != 0) {
        return false;
    }

    int day, month, year;
    int count = sscanf(dateStr, "%d.%d.%d", &day, &month, &year);

    if (count != 3 && strcmp(dateStr, "��� ������") != 0) {
        return false;
    }
    if (strcmp(dateStr, "��� ������") == 0)
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

// ������� �������� ����� ����� �� ������������ �������
bool isValidFileName(const char* fileName) {
    const char* invalidChars = "\\/:*?\"<>|";

    // ���������
    for (int i = 0; fileName[i] != '\0'; ++i) {
        for (int j = 0; invalidChars[j] != '\0'; ++j) {
            if (fileName[i] == invalidChars[j]) {
                return false;
            }
        }
    }

    return true;
}

// ������� ������ ����������
void instruction() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
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
            printf("%s\n", current_path);
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

// ������� �������� ����� ��� �������� (Ctrl+V)
void openFileForAppend(FILE*& file, const char* full_name, const char* headers, char* mode, size_t mode_size) {

    if (_access(full_name, 0) == 0) {
        SAFE_STRCPY(mode, "�������� ����� (��������)", mode_size);
    }
    else {
        SAFE_STRCPY(mode, "�������� �����", mode_size);
    }

    file = fopen(full_name, "a");
    if (!file) {
        printf("�� ������� ������� ���� ��� ������. ��������, ���� ����� ������ ����������, ��� � ��� ��� ���� �������.\n");
        printf("��� ����������� ������� Enter.");
        system("PAUSE>nul");
        return;
    }

    system("cls");
    printf("���� ������ ��� ��������.\n\n");
    printf("���� � �����: %s\n\n", full_name);
    printf("������� TAB ��� �������� � ���������� ���������.\n");
    printf("������� Enter ��� �������� � ���������� ���������.\n");
    printf("�� ������ ������ '��� ������' � ����� ����.\n");


    char fields[3][1000]; // ������ ��� �������� �����
    int current_col = 0;

    while (true) {
        for (current_col = 0; current_col < 3; current_col++) {
            int i = 0;
            memset(fields[current_col], 0, sizeof(fields[current_col]));

            if (current_col == 0) {
                if (strcmp(headers, "��� ��������������� | ���� ���������� | �������� �����������\n") == 0) {
                    printf("\n��� ���������������: ");
                }
                else {
                    printf("\n�������� �����������: ");
                }
            }
            else if (current_col == 1) {
                if (strcmp(headers, "��� ��������������� | ���� ���������� | �������� �����������\n") == 0) {
                    printf("\n���� ���������� (��.��.����): ");
                }
                else {
                    printf("\n�����: ");
                }
            }
            else if (current_col == 2) {
                if (strcmp(headers, "��� ��������������� | ���� ���������� | �������� �����������\n") == 0) {
                    printf("\n�������� �����������: ");
                }
                else {
                    printf("\n������� ������������: ");
                }
            }

            while (true) {
                int c = _getch();

                if (c == 27) {  // ESC
                    if (file) fclose(file);
                    return;
                }
                else if (c == '\t' || c == '\r') { // Tab or Enter
                    fields[current_col][i] = '\0';

                    if (current_col == 1 && strcmp(headers, "��� ��������������� | ���� ���������� | �������� �����������\n") == 0) {
                        if (!isValidDate(fields[current_col])) {
                            printf("\n������������ ������ ����. ������� ���� � ������� '��.��.����' (��������, 01.01.2023) ��� ������� '��� ������'.\n");
                            i = 0;
                            memset(fields[current_col], 0, sizeof(fields[current_col]));
                            continue;
                        }
                    }

                    if (i == 0 && strcmp(fields[current_col], "��� ������") != 0)
                    {
                        printf("\n�� �� ����� ������� ������. ����������, ������� ���������� ��� ������� '��� ������'.\n");
                        continue;
                    }
                    break;
                }
                else if (c == 8) { // Backspace
                    if (i > 0) {
                        i--;
                        printf("\b \b");
                        fields[current_col][i] = '\0';
                    }
                }
                else if (c == 22) // Ctrl + V
                {
                    if (OpenClipboard(NULL)) {
                        HANDLE hData = GetClipboardData(CF_TEXT);
                        if (hData != NULL) {
                            char* pszText = (char*)GlobalLock(hData);
                            if (pszText != NULL) {
                                // �������� �� ������ ������
                                for (int j = 0; pszText[j] != '\0' && i < 999; ++j) {
                                    if (pszText[j] >= 32 && pszText[j] <= 255)
                                    {
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
                else if (c == 0 || c == 224) { // Special keys
                    _getch();
                }
                else if (c >= 32 && c <= 255) {
                    if (i < 999)
                    {
                        fields[current_col][i++] = (char)c;
                        printf("%c", c);
                    }
                }
            }
        }
        // ������ � ����
        fprintf(file, "%s; %s; %s;\n", fields[0], fields[1], fields[2]);
        fflush(file);
        printf("\n");
    }
}


// ������� ��� ������ � ��������� ����������������
void outgoing_correspondence() {
    char txt_name[256];
    char full_name[512];
    char mode[256];

    do {
        system("cls");
        SetConsoleCP(1251);
        SetConsoleOutputCP(1251);
        printf("��������� ���������������:\n");
        printf("1.   �������/������� ���� ��� ��������\n");
        printf("2.   ������������ ����\n");
        printf("Esc. �����\n");
        printf("_____________________________________________\n\n");
        char menu_out_case = _getch();
        if (menu_out_case == 27) {
            return;
        }

        FILE* file;
        switch (menu_out_case) {
        case '1': {
            printf("\n");
            printf("������� ��� �����: ");
            getLineWithEsc("", txt_name, sizeof(txt_name));
            if (txt_name[0] == '\0') {
                break;
            }

            if (!isValidFileName(txt_name)) {
                printf("������������ ������� � ����� �����. ����������� ������ �����, �����, ������� � ����� ����������.\n");
                printf("��� ����������� ������� Enter."); system("PAUSE>nul");
                break;
            }

            snprintf(full_name, sizeof(full_name), "%s%s%s%s", folder_way, ocfe, txt_name, file_extension);
            openFileForAppend(file, full_name, "��� ��������������� | ���� ���������� | �������� �����������\n", mode, sizeof(mode));
            break;
        }
        case '2': {
            printf("\n");
            printf("������� ��� ����� ��� ����������: ");
            getLineWithEsc("", txt_name, sizeof(txt_name));

            if (txt_name[0] == '\0') {
                break;
            }
            if (!isValidFileName(txt_name)) {
                printf("������������ ������� � ����� �����. ����������� ������ �����, �����, ������� � ����� ����������.\n");
                printf("��� ����������� ������� Enter."); system("PAUSE>nul");
                break;
            }

            snprintf(full_name, sizeof(full_name), "%s%s%s%s", folder_way, ocfe, txt_name, file_extension);

            if (_access(full_name, 0) == 0) {
                printf("���� ����������.  ������������? (y/n): ");
                char overwriteConfirm = _getch();
                if (tolower(overwriteConfirm) != 'y') {
                    printf("\n���� �� ��� �����������.\n");
                    printf("��� ����������� ������� Enter."); system("PAUSE>nul");
                    break;
                }
                printf("\n");
                SAFE_STRCPY(mode, "���������� �����", sizeof(mode));
            }
            else {
                SAFE_STRCPY(mode, "�������� �����", sizeof(mode));
            }

            file = fopen(full_name, "w");
            if (file) {
                fclose(file);
                printf("���� ������� �����������.\n");
                openFileForAppend(file, full_name, "��� ��������������� | ���� ���������� | �������� �����������\n", mode, sizeof(mode));
            }
            else {
                printf("������ �������� ����� ��� ������. ��������, ���� ����� ������ ����������, ��� � ��� ��� ���� �������.\n");
                printf("��� ����������� ������� Enter."); system("PAUSE>nul");
            }
            break;
        }
        default:
            printf("������������ ����. �������� 1, 2 ��� Esc.\n");
            printf("��� ����������� ������� Enter."); system("PAUSE>nul");
            break;
        }
    } while (true);
}

// ������� ��� ������ � �������� �����������
void organization_addresses() {
    char txt_name[256];
    char full_name[512];
    char mode[256];

    do {
        system("cls");
        SetConsoleCP(1251);
        SetConsoleOutputCP(1251);
        printf("������ �����������:\n");
        printf("1.  �������/������� ���� ��� ��������\n");
        printf("2.  ������������ ����\n");
        printf("Esc. �����\n");
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
            printf("������� ��� �����: ");
            getLineWithEsc("", txt_name, sizeof(txt_name));
            if (txt_name[0] == '\0') {
                break;
            }

            if (!isValidFileName(txt_name)) {
                printf("������������ ������� � ����� �����. ����������� ������ �����, �����, ������� � ����� ����������.\n");
                printf("��� ����������� ������� Enter."); system("PAUSE>nul");
                break;
            }

            snprintf(full_name, sizeof(full_name), "%s%s%s%s", folder_way, oa, txt_name, file_extension);
            openFileForAppend(file, full_name, "�������� ����������� | ����� | ������� ������������\n", mode, sizeof(mode));
            break;
        }
        case '2': {
            printf("\n");
            printf("������� ��� ����� ��� ����������: ");
            getLineWithEsc("", txt_name, sizeof(txt_name));
            if (txt_name[0] == '\0') {
                break;
            }

            if (!isValidFileName(txt_name)) {
                printf("������������ ������� � ����� �����. ����������� ������ �����, �����, ������� � ����� ����������.\n");
                printf("��� ����������� ������� Enter."); system("PAUSE>nul");
                break;
            }

            snprintf(full_name, sizeof(full_name), "%s%s%s%s", folder_way, oa, txt_name, file_extension);

            if (_access(full_name, 0) == 0) {
                printf("���� ����������. ������������? (y/n): ");
                char overwriteConfirm = _getch();
                if (tolower(overwriteConfirm) != 'y') {
                    printf("\n���� �� ��� �����������.\n");
                    printf("��� ����������� ������� Enter."); system("PAUSE>nul");
                    break;
                }
                printf("\n");
                SAFE_STRCPY(mode, "���������� �����", sizeof(mode));
            }
            else {
                SAFE_STRCPY(mode, "�������� �����", sizeof(mode));
            }

            file = fopen(full_name, "w");
            if (file) {
                fclose(file);
                printf("���� ������� �����������.\n");
                openFileForAppend(file, full_name, "�������� ����������� | ����� | ������� ������������\n", mode, sizeof(mode));
            }
            else {
                printf("������ �������� ����� ��� ������. ��������, ���� ����� ������ ����������, ��� � ��� ��� ���� �������.\n");
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
                break;
            }
            else {
                printf("������� �������� ���� (����� 2).\n");
                printf("��� ����������� ������� Enter."); system("PAUSE>nul");
            }
            break;
        case '2':
            if (folder_way[0] != '\0') {
                organization_addresses();
                break;
            }
            else {
                printf("������� �������� ���� (����� 2).\n");
                printf("��� ����������� ������� Enter."); system("PAUSE>nul");
            }
            break;

        default:
            break;
        }
    } while (true);
}

// ������� ������������ ����
void normalizePath(const char* path, char* normalized_path, size_t normalized_path_size) {
    char current_dir[256];

    if (_getcwd(current_dir, sizeof(current_dir)) == NULL) {
        fprintf(stderr, "�� ������� ���������� ������� �����.\n");
        normalized_path[0] = '\0';
        return;
    }

    if (path[0] == '\0') {
        SAFE_STRCPY(normalized_path, current_dir, normalized_path_size);
        return;
    }

    // ���������� ����
    if ((isalpha(path[0]) && path[1] == ':')) {
        SAFE_STRCPY(normalized_path, path, normalized_path_size);
        // ������� ������������� �����
        int j = 0;
        for (int i = 0; normalized_path[i] != '\0'; ++i) {
            if (normalized_path[i] == '\\') {
                // ���������� ������������� �����
                while (normalized_path[i + 1] == '\\') {
                    i++;
                }
            }
            normalized_path[j++] = normalized_path[i];
        }
        normalized_path[j] = '\0';

        return;
    }
    // ������������� ����
    char temp_path[256] = { 0 };
    SAFE_STRCPY(temp_path, current_dir, sizeof(temp_path));

    char* token = strtok((char*)path, "\\/");
    while (token != NULL) {
        if (strcmp(token, ".") == 0) {
            // ������� ����������
        }
        else if (strcmp(token, "..") == 0) {
            // ����� �� ��������
            char* last_sep = strrchr(temp_path, '\\');

            if (last_sep != NULL) {
                *last_sep = '\0';
            }
        }
        else {
            // ���������� ����������
            size_t len = strlen(temp_path);
            if (len > 0 && temp_path[len - 1] != '\\') {
                strncat(temp_path, "\\", sizeof(temp_path) - len - 1);
            }
            strncat(temp_path, token, sizeof(temp_path) - strlen(temp_path) - 1);
        }
        token = strtok(NULL, "\\/");
    }
    SAFE_STRCPY(normalized_path, temp_path, normalized_path_size);
}

// ������� ������ ���� � �����
void program_way() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    printf("\n");
    char folder_way_new[256] = { 0 };
    char normalized_path[256];

    printf("������� ���� � �����: ");
    getLineWithEsc("", folder_way_new, sizeof(folder_way_new));
    if (folder_way_new[0] == '\0') {
        return;
    }
    unsigned char c = folder_way_new[0];
    if ((c >= 192 && c <= 223) || (c >= 224 && c <= 255)) {
        printf("������ �����. �������� ����. \n");
        printf("������������ ��������� ���������� ����: %s\n", folder_way);
        printf("��� ����������� ������� Enter."); system("PAUSE>nul");
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
            printf("���� � ����� ������.\n");

            // ��������� ����������� ����, ���� ��� ���
            size_t len = strlen(normalized_path);
            if (len > 0 && normalized_path[len - 1] != '\\' && normalized_path[len - 1] != '/') {
                snprintf(folder_way, sizeof(folder_way), "%s\\", normalized_path);
            }
            else {
                SAFE_STRCPY(folder_way, normalized_path, sizeof(folder_way));
            }

        }
        else {
            printf("��� ���� ��� ������� � ��������� �����. �������� ������ ����� ��� ���������� � ��������������.\n");
            if (folder_way[0] != '\0') {
                printf("������������ ��������� ���������� ����: %s\n", folder_way);
            }
        }
    }
    else {
        printf("��������� ���� �� ����������. ��������� ������������ ��������� � ���������, ��� ����� ����������.\n");
        if (folder_way[0] != '\0') {
            printf("������������ ��������� ���������� ����: %s\n", folder_way);
        }
    }
    printf("��� ����������� ������� Enter."); system("PAUSE>nul");
}

// ������� ���� ���������
void menu() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // ��������������� ���� ��� �������
    if (folder_way[0] == '\0') {
        char current_path[256];
        if (_getcwd(current_path, sizeof(current_path)) != NULL)
        {
            snprintf(folder_way, sizeof(folder_way), "%s\\", current_path);
        }
        else
        {
            printf("�� ������� ���������� ������� �����.\n");
        }
    }

    printf("\n��������� ��� ������� ���� ������.\n");

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
                break;
            }
            else {
                printf("������� �������� ���� (����� 2).\n");
                printf("��� ����������� ������� Enter."); system("PAUSE>nul");
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