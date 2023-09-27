#include <iostream>

// Функція для додавання рядка до списку Array
void addString(char*** Array, int* ArrayLines, char* buffer) {
    if (*ArrayLines == 0) {
        *Array = (char**)malloc(sizeof(char*));
        (*Array)[*ArrayLines] = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
        strcpy((*Array)[*ArrayLines], buffer);
        (*ArrayLines)++;
    } else {
        (*Array)[*ArrayLines - 1] = (char*)realloc((*Array)[*ArrayLines - 1], (strlen((*Array)[*ArrayLines - 1]) + strlen(buffer) + 1) * sizeof(char));
        strcat((*Array)[*ArrayLines - 1], buffer);
    }
}

// Функція для виведення всіх рядків зі списку Array
void printStrings(char** Array, int ArrayLines) {
    for (int i = 0; i < ArrayLines; i++) {
        printf("%d: %s\n", i + 1, Array[i]);
    }
}
// Функція для збереження списку Array у файл
void saveToFile(char** Array, int ArrayLines, char* FileName) {
    FILE* file = fopen(FileName, "w");
    if (file != NULL) {
        for (int i = 0; i < ArrayLines; i++) {
            fprintf(file, "%s\n", Array[i]);
        }
        fclose(file);
        printf("Array saved to %s\n", FileName);
    } else {
        printf("Error opening the file.\n");
    }
}

// Функція для завантаження списку Array з файлу
void loadFromFile(char*** Array, int* ArrayLines, char* FileName) {
    FILE* file = fopen(FileName, "r");
    if (file != NULL) {
        char line[400];
        while (fgets(line, sizeof(line), file) != NULL)
        {
            line[strlen(line) - 1] = '\0';
            (*ArrayLines)++;
            (*Array) = (char**)realloc((*Array), (*ArrayLines) * sizeof(char*));
            (*Array)[(*ArrayLines) - 1] = (char*)malloc((strlen(line) + 1) * sizeof(char));
            strcpy((*Array)[(*ArrayLines) - 1], line);
        }
        fclose(file);
        printf("Array loaded from %s\n", FileName);
    } else {
        printf("Error opening the file.\n");
    }
}

// Функція для пошуку та виведення підрядка в рядку із зазначенням індексу рядка
void searchSubstringInLine(char* line, int line_index, char* substring) {
    int found = 0;
    int position = -1;
    int first_letter_pos = -1;
    int last_letter_pos = -1;
    int substring_index = 0;

    for (int j = 0; j < strlen(line); j++) {
        if (line[j] == substring[substring_index]) {
            if (first_letter_pos == -1) {
                first_letter_pos = j;
            }
            last_letter_pos = j;
            substring_index++;

            if (substring_index == strlen(substring)) {
                position = first_letter_pos;
                printf("Word found in line %d, positions %d to %d: %s\n", line_index, position + 1, last_letter_pos + 1, substring);
                found = 1;
                substring_index = 0;
                first_letter_pos = -1;
            }
        } else {
            substring_index = 0;
            first_letter_pos = -1;
            last_letter_pos = -1;
        }
    }

    if (!found) {
        printf("Substring not found in line %d.\n", line_index);
    }
}

// Функція для пошуку тексту в масиві рядків
void searchSubstringInArray(char** Array, int ArrayLines, char* substring) {
    int found = 0;

    for (int i = 0; i < ArrayLines; i++) {
        char* line = Array[i];
        searchSubstringInLine(line, i + 1, substring);
        found = 1;
    }

    if (!found) {
        printf("Substring not found in any line.\n");
    }
}


// Функція для вставки підрядка в рядок за індексом та позицією
void insertSubstring(char*** Array, int ArrayLines, int line_index, int position, char* substring) {
    if (line_index < 1 || line_index > ArrayLines) {
        printf("Invalid line index.\n");
        return;
    }

    if (position < 0 || position > strlen((*Array)[line_index - 1])) {
        printf("Invalid position.\n");
        return;
    }

    int original_length = strlen((*Array)[line_index - 1]);
    int new_length = original_length + strlen(substring) + 1;

    int remaining_length = original_length - position;

    (*Array)[line_index - 1] = (char*)realloc((*Array)[line_index - 1], new_length * sizeof(char));

    memmove((*Array)[line_index - 1] + position + strlen(substring), (*Array)[line_index - 1] + position, remaining_length);

    strncpy((*Array)[line_index - 1] + position, substring, strlen(substring));
}


int main() {
    int command = 0;

    char** Array = NULL;
    int ArrayLines = 0;

    char FileName[50];

    while (1) {
        printf("Write command 1-7:");
        scanf("%d", &command);

        while (getchar() != '\n');

        switch (command) {
            case 1: {
                char buffer[400];
                printf("Write text to append:");
                scanf("%[^\n]", buffer);
                addString(&Array, &ArrayLines, buffer);
                break;
            }
            case 2: {
                ArrayLines++;
                Array = (char**)realloc(Array, ArrayLines * sizeof(char*));
                Array[ArrayLines - 1] = (char*)malloc(1);
                Array[ArrayLines - 1][0] = '\0';
                break;
            }
            case 3: {
                printStrings(Array, ArrayLines);
                break;
            }
            case 4: {
                printf("Write file name to SAVE:");
                scanf("%s", FileName);
                saveToFile(Array, ArrayLines, FileName);
                break;
            }
            case 5: {
                printf("Write file name to LOAD:");
                scanf("%s", FileName);
                loadFromFile(&Array, &ArrayLines, FileName);
                break;
            }
            case 6: {
                char substring[100];
                printf("Enter the substring to search for: ");
                scanf("%s", substring);
                searchSubstringInArray(Array, ArrayLines, substring);
                break;
            }
            case 7: {
                int line_index, position;
                char substring[100];

                printf("Enter line index for insertion: ");
                scanf("%d", &line_index);

                printf("Enter position for insertion (0-%d): ", strlen(Array[line_index - 1]));
                scanf("%d", &position);

                printf("Enter substring to insert: ");
                while (getchar() != '\n');
                fgets(substring, sizeof(substring), stdin);
                substring[strcspn(substring, "\n")] = '\0';

                insertSubstring(&Array, ArrayLines, line_index, position, substring);
                break;
            }


            case 8: {
                system("clear");
            }

            default: {
                if (command < 0 || command > 8) {
                    printf("The command is not implemented.\n");
                }
                break;
            }
        }
    }
    return 0;
}
