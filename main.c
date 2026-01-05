#include "common.h"
#include "utils.h"
#include "auth.h"
#include "quiz_engine.h"

int main() {
    srand(time(NULL));
    char username[30];
    int choice;

    while (1) {
        clearscreen();
        border(BLUE);
        printf("\n");
        centered("QUIZ MASTER", BOLD YELLOW);
        printf("\n");
        border(BLUE);

        printf("\n" YELLOW "1." RESET " LOGIN\n");
        printf(YELLOW "2." RESET " SIGN UP\n");
        printf(YELLOW "3." RESET " EXIT\n");
        printf("\n" CYAN "Enter choice: " RESET);
        
        choice = safe_input(1, 3);

        if (choice == 3) break;
        
        int status = (choice == 1) ? login(username) : signup(username);

        if (status == 1) {
            // User is logged in, show Quiz Menu
            while (1) {
                clearscreen();
                border(GREEN);
                centered("USER DASHBOARD", BOLD YELLOW);
                border(GREEN);
                printf("\n" YELLOW "1." RESET " TAKE QUIZ\n");
                printf(YELLOW "2." RESET " VIEW PROFILE\n");
                printf(YELLOW "3." RESET " PAST ATTEMPTS\n");
                printf(YELLOW "4." RESET " LEADERBOARD\n");
                printf(YELLOW "5." RESET " LOGOUT\n");
                
                int sub = safe_input(1, 5);
                if (sub == 1) take_quiz(username);
                else if (sub == 2) user_profile(username);
                else if (sub == 3) past_attempts(username);
                else if (sub == 4) leaderboard();
                else break;
            }
        }
    }
    return 0;
}