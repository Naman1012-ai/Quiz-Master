#include "quiz_engine.h"
#include "utils.h"
#include "auth.h"

void display_timer(int seconds, int posy) {
    gotoxy(0, posy);
    if (seconds <= 5) printf(RED BOLD "TIME LEFT:- "RESET"%02d seconds " , seconds);
    else if (seconds <= 10) printf(YELLOW BOLD "TIME LEFT:- "RESET"%02d seconds " , seconds);
    else printf(GREEN "TIME LEFT:- "RESET"%02d seconds ", seconds);
    fflush(stdout);
}

char get_validans_with_timer(int *time_left, int timer_pos)
{
    char ch;
    time_t start_time = time(NULL);
    int last_display = *time_left;

    while (1)
    {
        // Update timer
        time_t current_time = time(NULL);
        int elapsed = (int)difftime(current_time, start_time);
        *time_left = last_display - elapsed;

        if (*time_left <= 0)
        {
            *time_left = 0;
            display_timer(0, timer_pos);
            return '\0'; // Time's up
        }

        display_timer(*time_left, timer_pos);

        // Check for keyboard input (non-blocking)
        if (_kbhit())
        {
            ch = getch();
          
            if (ch == 'A' || ch == 'B' || ch == 'C' || ch == 'D' ||
            ch == 'a' || ch == 'b' || ch == 'c' || ch == 'd')
            {
                return ch;
            }
            else if (ch != '\r' && ch != '\n')
            {
                gotoxy(0, timer_pos + 2);
                printf(BG_RED "Invalid input! \n" GREEN "Please Enter " RESET "A, B, C " GREEN "or" RESET " D" GREEN":- " RESET);
            }
        }

        Sleep(100); // Small delay to prevent high CPU usage
    }
}

void save_attempts(attempts *att)
{
    FILE *fp = fopen(DATAFILE, "a");
    if (fp == NULL)
        return;

    FILE *fp1 = fopen(DATAFILE, "r");
    int attmptnum = 1;
    char line[200];
    char search_block[50];
    sprintf(search_block, "[ATTEMPTS: %s", att->username);

    if (fp != NULL)
    {
        while (fgets(line, sizeof(line), fp1))
        {
            if (strstr(line, search_block))
            {
                attmptnum++;
            }
        }
        fclose(fp1);
    }

    int high_score = att->totalques;
    fprintf(fp, "\n[ATTEMPTS: %s : %d]\n", &att->username, attmptnum);
    fprintf(fp, "SUBJECT: %s\n", att->subject);
    fprintf(fp, "TOTAL_QUESTIONS: %d\n", att->totalques);
    fprintf(fp, "SCORE: %0.2f/%d\n", att->score, high_score * 2);
    fprintf(fp, "PERCENTAGE: %0.2f\n", att->percentage);
    fprintf(fp, "TIME: %s\n", att->timestamps);

    fclose(fp);
}

void user_profile(char *username)
{
    clearscreen();
    border(CYAN);
    centered("YOUR PROFILE", BOLD YELLOW);
    border(CYAN);
    printf("\n");

    user u;
    load_userdata(username, &u);
    printf(BLUE "USERNAME        :-" RESET " %s\n", u.username);
    printf(BLUE "TOTAL ATTEMPTS  :-" RESET " %d\n", u.attempts);
    printf(BLUE "HIGHEST SCORE   :-" RESET " %0.2f\n", u.highest_score);

    printf("\n");
    printf("Press any key to return...\n");
    getch();
}


// #=== TO SEE PAST ATTEMPTS OF THE USER ===#
void past_attempts(char *username)
{
    clearscreen();
    border(BOLD GREEN);
    centered("PAST ATTEMPTS", BOLD YELLOW);
    border(BOLD GREEN);
    printf("\n");

    FILE *fp = fopen(DATAFILE, "r");
    if (fp == NULL)
    {
        printf(BG_RED " No past attempts found!\n" RESET);
        printf("Press any key to return...\n");
        getch();
        return;
    }

    char line[200];
    char search_block[50];
    sprintf(search_block, "[ATTEMPTS: %s", username);
    int found = 0;
    int attmptnum = 0;

    attempts att;
    memset(&att, 0, sizeof(att));
    strcpy(att.username, username);

    while (fgets(line, sizeof(line), fp))
    {
        if (strstr(line, search_block))
        {
            found = 1;
            attmptnum++;

            memset(&att, 0, sizeof(att));
            strcpy(att.username, username);
        
            while (fgets(line, sizeof(line), fp))
            {
                if (line[0] == '[')
                    break;

                else if (strstr(line, "SUBJECT: "))
                {
                    sscanf(line, "SUBJECT: %[^\n]", att.subject);
                }
                else if (strstr(line, "TOTAL_QUESTIONS: "))
                {
                    sscanf(line, "TOTAL_QUESTIONS: %d", &att.totalques);
                }
                else if (strstr(line, "SCORE: "))
                {
                    sscanf(line, "SCORE: %f", &att.score);
                }
                else if (strstr(line, "PERCENTAGE: "))
                {
                    sscanf(line, "PERCENTAGE: %f", &att.percentage);
                }
                else if (strstr(line, "TIME: "))
                {
                    sscanf(line, "TIME: %[^\n]", att.timestamps);
                }

            }

            int high_score = att.totalques;
            printf(YELLOW "ATTEMPT #%d\n", attmptnum);
            printf(CYAN "SUBJECT         :- " RESET "%s\n", att.subject);
            printf(CYAN "TOTAL QUESTIONS :- " RESET "%d\n", att.totalques);
            printf(CYAN "SCORE           :- " RESET "%0.2f/%d\n", att.score, high_score * 2);
            printf(CYAN "PERCENTAGE      :- " RESET "%0.2f\n", att.percentage);
            printf(CYAN "TIME            :- " RESET "%s\n", att.timestamps);
            printf("\n");
        }
    }
    fclose(fp);

    if (!found)
    {
        printf(BG_RED "No attemps found!\n" RESET);
    }

    printf("Press any key to return...\n");
    getch();
}

// #=== UPDATING LEADERBOARAD ===#
void update_leaderboard(char *username, float score, char *timestamps, int numques, char *subject)
{
    FILE *fp = fopen(LEADERBOARD_FILE, "a");
    if (fp == NULL)
        return;

    fprintf(fp, "%s| SCORE:- %0.2f/%d | SUBJECT:- %s |  %s\n", username, score, numques * 2, subject, timestamps);
    fclose(fp);
}


// #=== LEADERBOARD TO COMPETE ===#
void leaderboard()
{
    clearscreen();
    border(MAGENTA);
    centered("GLOABL LEADERBOARD", BOLD YELLOW);
    border(MAGENTA);
    printf("\n");

    FILE *fp = fopen(DATAFILE, "r");
    if (fp == NULL)
    {
        printf(BG_RED "NO DATA AVAILABLE!\n" RESET);
        printf("Press any key to return...\n");
        getch();
        return;
    }

    struct
    {
        char username[30];
        float scores;
    } score[100];

    int count = 0;

    char line[300];
    while (fgets(line, sizeof(line), fp) && count < 100)
    {
        if (strstr(line, "[USER: "))
        {
            char username[30];
            sscanf(line, "[USER: %[^]]]", username);

            float highest = 0.00;
            while (fgets(line, sizeof(line), fp))
            {
                if (line[0] == '[')
                    break;
                if (strstr(line, "HIGHEST_SCORE: "))
                {
                    sscanf(line, "HIGHEST_SCORE: %f", &highest);
                }
            }

            if (highest > 0)
            {
                strcpy(score[count].username, username);
                score[count].scores = highest;
                count++;
            }
        }
    }
    fclose(fp);

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (score[j].scores < score[j + 1].scores)
            {
                char tempname[30];
                strcpy(tempname, score[j].username);
                strcpy(score[j].username, score[j + 1].username);
                strcpy(score[j + 1].username, tempname);

                float tempscore = score[j].scores;
                score[j].scores = score[j + 1].scores;
                score[j + 1].scores = tempscore;
            }
        }
    }

    printf(BOLD " Rank   Username             Highest Score\n" RESET);
    printf("------------------------------------------\n");

    int display = (count < 10) ? count : 10;
    for (int i = 0; i < display; i++)
    {
        if (i == 0)
            printf(YELLOW);
        else if (i == 1)
            printf(GREEN);
        else if (i == 2)
            printf(CYAN);
        else
            printf(WHITE);

        printf(" %-6d %-20s %.2f\n", i + 1, score[i].username, score[i].scores);
        printf(RESET);
    }

    if (count == 0)
    {
        printf(BG_RED "No scores yet!\n" RESET);
    }

    printf("\n");
    printf("Press any key to return...\n");
    getch();
}

// #=== SWAPPING AND VALIDATING OF QUESTIONS (FROM 784 TO 825) ===#
void swapques(quiz *a, quiz *b)
{
    quiz temp = *a;
    *a = *b;
    *b = temp;
}

void shuffleques(quiz *ques, int n)
{
    for (int i = n - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        if (j != i)
        {
            swapques(&ques[i], &ques[j]);
        }
    }
}

int vldques(quiz *q)
{
    // Check empty strings
    if (strlen(q->question) < 2)
        return 0;
    if (strlen(q->answerA) < 2)
        return 0;
    if (strlen(q->answerB) < 2)
        return 0;
    if (strlen(q->answerC) < 2)
        return 0;
    if (strlen(q->answerD) < 2)
        return 0;

    // Check correct answer validity
    char c = q->correctanswer;
    if (c != 'A' && c != 'B' && c != 'C' && c != 'D' &&
        c != 'a' && c != 'b' && c != 'c' && c != 'd')
        return 0;

    return 1; // VALID
}

// #== PRINTING QUESTIONS, SHOWING RESULT AT END, AND SAVING THEM ===#
int diffsub(char *quesfile, char *username, char *subject, int difficulty)
{
    clearscreen();

    quiz ques[50];
    int totaloaded = 0;

    FILE *fp = fopen(quesfile, "r");

    if (fp == NULL)
    {
        printf(BG_RED "No questions in the file!\n" RESET);
        printf("Press any key to return...\n");
        getch();
        return 1;
    }

    while (fgets(ques[totaloaded].question, 200, fp) && totaloaded < 50)
    {
        if (!fgets(ques[totaloaded].answerA, 100, fp))
            break;
        if (!fgets(ques[totaloaded].answerB, 100, fp))
            break;
        if (!fgets(ques[totaloaded].answerC, 100, fp))
            break;
        if (!fgets(ques[totaloaded].answerD, 100, fp))
            break;

        if (fscanf(fp, " %c", &ques[totaloaded].correctanswer) != 1)
            break;
        fgetc(fp);

        if (vldques(&ques[totaloaded]) == 0)
        {
            printf(BG_RED "⚠ Skipping malformed question %d\n" RESET, totaloaded + 1);
        }
        totaloaded++;
    }
    fclose(fp);

    if (totaloaded == 0)
    {
        printf(BG_RED "\n[ERROR] No Questions Loaded from %s\n" RESET, quesfile);
        printf("Press any key to continue...\n");
        getch();
        return 1;
    }

    int numques, allowed_time;
    if (difficulty == 1)
    {
        allowed_time = 120;
        if(strcmp(subject, "MATHEMATICS") == 0) allowed_time = 180;
        numques = (totaloaded < 10) ? totaloaded : 10;
    }
    else if (difficulty == 2)
    {
        allowed_time = 360;
        if(strcmp(subject, "MATHEMATICS") == 0) allowed_time = 480;
        numques = (totaloaded < 20) ? totaloaded : 20;
    }
    else
    {
        allowed_time = 480;
        if(strcmp(subject, "MATHEMATICS") == 0) allowed_time = 600;
        numques = (totaloaded < 30) ? totaloaded : 30;
    }

    shuffleques(ques, totaloaded);

    border(BLUE);
    char header[30];
    sprintf(header, "%s QUIZ - LEVEL %d", subject, difficulty);
    centered(header, BOLD YELLOW);
    border(BLUE);
    printf("\n");

    printf(CYAN     "TOTAL QUESTIONS      "BOLD BLUE":- " RESET "%d\n", numques);
    printf(YELLOW   "TIME FOR QUIZ        "BOLD BLUE":- " RESET "%d minutes\n", allowed_time/60);
    printf(BG_GREEN "CORRECT ANSWER       "RESET BOLD BLUE":- " RESET "+2 points\n");
    printf(BG_RED   "'4' WRONG ANSWERs    "RESET BOLD BLUE":- " RESET "-1 points\n");
    printf(BG_PINK  "NOT ANSWERED         "RESET BOLD BLUE":- " RESET "-0.5 points\n");

    if(strcmp(subject, "MATHEMATICS") == 0){
        printf(BOLD MAGENTA "--> '^o' means degrees angle!!\n" RESET);
    }
    printf("\nPress any key to start...\n");
    getch();

    float score = 0.00;
    int not_answered = 0;
    int crct_ans = 0;
    int wrng_ans = 0;
    int timer_line = 9;
    int time_left = allowed_time;

    for (int i = 0; i < numques; i++)
    {
        clearscreen();
        printf(CYAN "\nQUESTION %d/%d\n" RESET, i + 1, numques);
        printf(BOLD "%s" RESET, ques[i].question);
        printf(WHITE "%s" RESET, ques[i].answerA);
        printf(WHITE "%s" RESET, ques[i].answerB);
        printf(WHITE "%s" RESET, ques[i].answerC);
        printf(WHITE "%s" RESET, ques[i].answerD);

        printf(YELLOW "Enter your answer:- " RESET);
        fflush(stdout);
        char useranswer = get_validans_with_timer(&time_left, timer_line);

        // CHECKING USER ANSWER
        if (useranswer == '\0')
        {
            // Time's up
            printf(RED BOLD "\n\nTIME'S UP!\n" RESET);
            printf(GREEN "Correct Answer:- " RESET "%c\n", ques[i].correctanswer);
            not_answered++;
            score -= 0.5;
        }
        else if (tolower(useranswer) == tolower(ques[i].correctanswer))
        {
            crct_ans++;
            printf(BOLD GREEN "\n\nYOU GOT IT CORRECT!\n" RESET);
            score += 2.0;
        }
        else if (tolower(useranswer) != tolower(ques[i].correctanswer))
        {
            wrng_ans++;
            printf(BOLD RED "\n\nOops! WRONG ANSWER!\n" RESET);
            printf(GREEN "Correct Answer:- " RESET "%c", ques[i].correctanswer);
        }

        printf("\nPress any key to next question...");
        getch();
    }

    int x = wrng_ans / 4;
    
    attempts att;
    strcpy(att.username, username);
    strcpy(att.subject, subject);
    att.score = score - x;
    att.totalques = numques;
    att.percentage = (score / (numques * 2.0)) * 100;
    if (att.percentage < 0)
        att.percentage = 0;
    get_timestamps(att.timestamps);

    save_attempts(&att);

    user u;
    load_userdata(username, &u);
    u.attempts++;
    if (score > u.highest_score)
    {
        u.highest_score = score;
    }
    save_userdata(&u);

    update_leaderboard(username, score, att.timestamps, numques, subject);

    clearscreen();
    border(GREEN);
    centered("QUIZ COMPLETED", BOLD YELLOW);
    border(GREEN);
    printf("\n");

    printf(CYAN       " SUBJECT         "BOLD BLUE":- " RESET "%s\n", subject);
    printf(CYAN       " TOTAL QUESTION  "BOLD BLUE":- " RESET "%d\n", numques);
    printf(CYAN       " YOUR SCORE      "BOLD BLUE":- " RESET "%0.2f/%d\n", score, numques * 2);
    printf(CYAN       " PERCENTAGE      "BOLD BLUE":- " RESET "%0.2f\n", att.percentage);
    printf(BOLD GREEN " CORRECT ANSWERS "BOLD BLUE":- "RESET"%d\n" , crct_ans);
    printf(BOLD RED   " WRONG ANSWER(S) "BOLD BLUE":- "RESET"%d\n" , wrng_ans);
    printf(BOLD YELLOW" NOT ANSWERED    "BOLD BLUE":- "RESET"%d\n" , not_answered);

    if (att.percentage > 80)
    {
        printf(BG_GREEN "\n EXCELLENT! \n" RESET);
    }
    else if (att.percentage > 60)
    {
        printf(BG_PINK "\n GOOD JOB! \n" RESET);
    }
    else if (att.percentage > 30)
    {
        printf(BG_BLUE "\n KEEP PRACTICING! \n" RESET);
    }
    else if (att.percentage < 30)
    {
        printf(BG_RED "\n DON'T GIVE UP! \n" RESET);
    }

    printf("\n");
    printf("Press any key to return...\n");
    getch();
    return 0;
}

// #=== ASKING USER FOR SUBJECT, LEVEL OF DIFICULTY ===#
void take_quiz(char *username)
{
    while (1)
    {
        clearscreen();
        border(MAGENTA);
        centered("CHOOSE SUBJECT", BOLD YELLOW);
        border(MAGENTA);
        printf("\n");

        printf(GREEN "1." RESET "English\n");
        printf(GREEN "2." RESET "Mathematics\n");
        printf(GREEN "3." RESET "Science\n");
        printf(GREEN "4." RESET "General Knowledge\n");
        printf(GREEN "5." RESET "Computer Science\n");
        printf(GREEN "6." RESET "Return ...\n");
        printf("\n");
        printf(CYAN "Enter your choice:- " RESET);
        int subchoice = safe_input(1, 6);

        if (subchoice == 6)
            return;

        char *filename;
        char subject[20];
        if (subchoice == 1)
        {
            filename = "eng.txt";
            strcpy(subject, "ENGLISH");
        }
        if (subchoice == 2)
        {
            filename = "math.txt";
            strcpy(subject, "MATHEMATICS");
        }
        if (subchoice == 3)
        {
            filename = "science.txt";
            strcpy(subject, "SCIENCE");
        }
        if (subchoice == 4)
        {
            filename = "gk.txt";
            strcpy(subject, "GK");
        }
        if (subchoice == 5)
        {
            filename = "cs.txt";
            strcpy(subject, "CS");
        }

        clearscreen();
        border(YELLOW);
        centered("CHOOSE DIFFICULTY", BOLD CYAN);
        border(YELLOW);
        printf("\n");

        printf(GREEN "1." RESET "EASY(10 questions)\n");
        printf(GREEN "2." RESET "MEDIUM(20 questions)\n");
        printf(GREEN "3." RESET "HARD(30 questions)\n");
        printf("\n");
        printf(CYAN "Enter your choice:- " RESET);
        int difficulty = safe_input(1, 3);

        int x = diffsub(filename, username, subject, difficulty);

        clearscreen();
        if(x != 1){
            printf(CYAN "WOULD YOU LIKE TO PLAY AGAIN ?\n" RESET);
            printf(GREEN "1." RESET "YES(same subject, new difficulty)\n");
            printf(GREEN "2." RESET "NO(return to homepage...)\n");
            printf("\n");
            printf(YELLOW "Enter your choice:- " RESET);
            int playagain = safe_input(1, 2);

            if (playagain == 2)
                return;

            if (playagain == 1)
            {
                clearscreen();
                border(YELLOW);
                centered("CHOOSE DIFFICULTY", BOLD CYAN);
                border(YELLOW);
                printf("\n");

                printf(GREEN "1." RESET "EASY(10 questions)\n");
                printf(GREEN "2." RESET "MEDIUM(20 questions)\n");
                printf(GREEN "3." RESET "HARD(30 questions)\n");
                printf("\n");
                printf(CYAN "Enter your choice:- " RESET);
                int difficulty = safe_input(1, 3);

                diffsub(filename, username, subject, difficulty);

                clearscreen();
                printf(CYAN "CONTINUE PLAYING ?\n" RESET);
                printf(GREEN "1." RESET "YES(new subject)\n");
                printf(GREEN "2." RESET "NO(return to homepage...)\n");
                printf("\n");
                printf(CYAN "Enter your choice:- " RESET);
                int conti = safe_input(1, 2);

                if (conti == 2)
                {
                    return;
                }
            }
        }
    }
}
