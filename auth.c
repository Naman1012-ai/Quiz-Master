#include "auth.h"
#include "utils.h"

void getmasked_password(char *pass, int maxlen) {
    int i = 0;
    char ch;
    while (1) {
        ch = getch();
        if (ch == 13) { // Enter
            pass[i] = '\0';
            printf("\n");
            break;
        } else if (ch == 8 && i > 0) { // Backspace
            i--;
            printf("\b \b");
        } else if (i < maxlen - 1) {
            pass[i++] = ch;
            printf("*");
        }
    }
}

int user_exist(char *username) {
    FILE *fp = fopen(DATAFILE, "r");
    if (fp == NULL) return 0;
    char line[300], userblock[50];
    sprintf(userblock, "[USER: %s]", username);
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, userblock)) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

// ... Paste your save_userdata, load_userdata, login, and signup functions here ...
// Make sure to include "auth.h" and "utils.h" at the top!