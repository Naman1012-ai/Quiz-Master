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

void save_userdata(struct user *u)
{
    FILE *fp = fopen(DATAFILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (fp == NULL)
    {
        fp = fopen(DATAFILE, "w");
        fprintf(fp, "\n\n[USER: %s]\n", u->username);
        fprintf(fp, "PASSWORD: %s\n", u->password);
        fprintf(fp, "TOTAL_ATTEMPTS: %d\n", u->attempts);
        fprintf(fp, "HIGHEST_SCORE: %0.2f\n\n", u->highest_score);
        fclose(fp);
        if(temp != NULL) fclose(temp);
        remove("temp.txt");
        return;
    }

    char line[200];
    char user_block[50];
    sprintf(user_block, "[USER: %s]", u->username);
    int found = 0;
    int in_user_block = 0;

    while (fgets(line, sizeof(line), fp))
    {
        if (strstr(line, user_block))
        {
            found = 1;
            in_user_block = 1;

            fprintf(temp, "\n\n[USER: %s]\n", u->username);
            fprintf(temp, "PASSWORD: %s\n", u->password);
            fprintf(temp, "TOTAL_ATTEMPTS: %d\n", u->attempts);
            fprintf(temp, "HIGHEST_SCORE: %0.2f\n\n", u->highest_score);

            continue;
        }
        if(in_user_block){
            if(line[0] == '['){
                in_user_block = 0;
                fprintf(temp, "%s", line);
            }
            continue;
        }
        fprintf(temp, "%s", line);
    }

    if (!found)
    {
        fprintf(temp, "\n\n[USER: %s]\n", u->username);
        fprintf(temp, "PASSWORD: %s\n", u->password);
        fprintf(temp, "TOTAL_ATTEMPTS: %d\n", u->attempts);
        fprintf(temp, "HIGHEST_SCORE: %0.2f\n\n", u->highest_score);
    }

    fclose(fp);
    fclose(temp);
    remove(DATAFILE);
    rename("temp.txt", DATAFILE);
}

// #=== LOADING USER'S DATA IN USER STRUCTRE ===#
void load_userdata(char *username, user *u)
{
    strcpy(u->username, username);
    u->attempts = 0;
    u->highest_score = 0.0;

    FILE *fp = fopen(DATAFILE, "r");
    if (fp == NULL)
        return;

    char line[200];
    char user_block[50];

    sprintf(user_block, "[USER: %s]", username);
    int found = 0;

    while (fgets(line, sizeof(line), fp))
    {
        if (strstr(line, user_block))
        {
            while (fgets(line, sizeof(line), fp))
            {
                if (line[0] == '[')
                    break;
                if (strstr(line, "PASSWORD: "))
                {
                    sscanf(line, "PASSWORD: %s", u->password);
                }
                else if (strstr(line, "TOTAL_ATTEMPTS: "))
                {
                    sscanf(line, "TOTAL_ATTEMPTS: %d", &u->attempts);
                }
                else if (strstr(line, "HIGHEST_SCORE: "))
                {
                    sscanf(line, "HIGHEST_SCORE: %f\n\n", &u->highest_score);
                }
            }
            break;
        }
    }
    fclose(fp);
}

// =============================
//   FUNCTIONS FOR MAIN SCREEN
// =============================

// #=== LOGIN FUNCTIONS FOR OLD USER ===#
int login(char *username)
{
    clearscreen();
    border(CYAN);
    centered("LOGIN", BOLD YELLOW);
    border(CYAN);
    printf("\n");

    printf(CYAN "Enter username " RESET "(To exit, press '1') " CYAN ":- ");
    scanf("%29s", username);
    while (getchar() != '\n');

    if(strcpy(username, "1") == 0) return 0;
    int choose;
    if (!user_exist(username))
    {
        printf(BG_RED "Username doesn't exist!" RESET);
        printf("\n\n");
        printf("Press any key to return...\n");
        getch();
        return 0; 
    }
    
    user u;
    load_userdata(username, &u);

    char password[15];
    int attempts = 0;

    while (attempts < 3)
    {
        printf(CYAN "Enter password:- " RESET);
        getmasked_password(password, 15);

        if (strcmp(password, u.password) == 0)
        {
            printf(BG_GREEN "\nLOGIN SUCCESSFULL!\n" RESET);
            printf("\nPress any key to continue...\n");
            getch();
            return 1;
        }

        attempts++;
        if (attempts < 3)
        {
            printf(BG_RED "Wrong Password! %d attempt(s) remaining.\n" RESET, 3 - attempts);
        }
    }
    printf(BG_RED "Too many wrong attempts!\n" RESET);
    printf("Press any key to continue...\n");
    getch();
    return 0;
}

// #=== SIGNUP FOR NEW USER ===#
int signup(char *username)
{
    clearscreen();
    border(CYAN);
    centered("SIGN UP", BOLD YELLOW);
    border(CYAN);
    printf("\n");

    int attempts = 0;

    printf(CYAN "Enter username " RESET "(To exit, press '1') " CYAN ":- ");
    scanf("%29s", username);
    while (getchar() != '\n');
    
    if(strcpy(username, "1") == 0) return 0;
    while (strlen(username) < 3)
    {
        printf(BG_RED "Username must be atleast 3 characters long!\n" RESET);
        printf("\n");
         printf(GREEN "Enter username " RESET "(To exit, press '1') "GREEN ":- ");
        scanf("%29s", username);
        while (getchar() != '\n');

        if(strcpy(username, "1") == 0) return 0;
        attempts++;     
      
        if (attempts == 2 && strlen(username) < 3)
        {
            printf(BG_RED "Too many wrong attempts!" RESET);
            printf("\n");
            printf("Press any key to return...\n");
            getch();
            return 0;
            break;
        }
    }

    attempts = 0;
    int choice;
    while(user_exist(username)){
        printf(BG_RED "Username already exist!" RESET);
        printf("\n\n");

        printf(PINK "--> CHOOSE WHAT TO DO NEXT!!" RESET );
        printf("\n");
        printf(YELLOW"1." RESET "SIGN UP WITH NEW ACCOUNT\n");
        printf(YELLOW"2." RESET "LOGIN INSTEAD");
        printf(YELLOW"3." RESET "RETURN...");
        printf("\n\n");
        printf(BLUE "Enter your choice" RESET ":- ");
        choice = safe_input(1, 3);

        if(choice == 1){
            clearscreen();
            border(CYAN);
            centered("SIGN UP", BOLD YELLOW);
            border(CYAN);
            printf("\n");
            printf(BLUE "\nEnter username" RESET ":- ");
            scanf("%19s", username);
        }

        else if(choice == 2){
            login(username);
            return 1;
        }
        else if(choice == 3){
            return 0;
        }
                
        attempts++;
        if (attempts == 2 && (user_exist(username)))
        {
            printf(BG_RED "\nAccount number already exist!" RESET);
            printf(RED "\nToo many wrong attempts! Try again later!!" RESET);
            printf("\n");
            printf("Press any key to return...\n");
            getch();
            return 0;
            break;
        }
    }

    char password[15];
    char confirm_password[15];
    attempts = 0;

    printf(GREEN "Set a Password:- " RESET);
    getmasked_password(password, 15);

    while (strlen(password) < 4)
    {
        printf(BG_RED "Password must be atleast 4 characters long!" RESET);
        printf("\n");
        printf("Press any key to continue...\n");
        getch();
        printf("\n\n");
        printf(GREEN "Set a Password:- " RESET);
        getmasked_password(password, 15);
        attempts++;

        if (attempts == 2)
        {
            printf(BG_RED "Too many wrong attempts!" RESET);
            printf("\n");
            printf("Press any key to return...\n");
            getch();
            return 0;
            break;
        }
    }

   attempts = 0;
    printf(CYAN "Confirm Password:- " RESET);
    getmasked_password(confirm_password, 15);

    while (strcmp(password, confirm_password) != 0)
    {
        printf("\n");
        printf(BG_RED "Password do not match!" RESET);
        printf("\n");
        printf("Press any key to continue...\n");
        getch();
        printf("\n");
        printf(CYAN "Confirm Password:- " RESET);
        getmasked_password(confirm_password, 15);

        attempts++;
        if (attempts == 2)
        {
            printf(BG_RED "Too many wrong attempts!" RESET);
            printf("\n");
            printf("Press any key to return...\n");
            getch();
            return 0;
            break;
        }
    }

    user u;
    strcpy(u.username, username);
    strcpy(u.password, password);
    u.attempts = 0;
    u.highest_score = 0.00;

    save_userdata(&u);

    printf(BOLD GREEN "\nAccount Created Successfully!\n\n" RESET);
    printf(BOLD YELLOW "Returning to loginpage..." RESET);
    printf("\nPress any key to continue...\n");
    getch();
    return 1;
}
