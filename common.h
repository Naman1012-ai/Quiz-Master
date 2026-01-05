#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <ctype.h>

// Color Codes
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define BOLD "\033[1m"
#define PINK "\033[95m"
#define BG_BLUE "\033[44m"
#define BG_GREEN "\033[42m"
#define BG_RED "\033[41m"
#define BG_PINK "\033[105m"

// Data Files
#define DATAFILE "users_data.txt"
#define LEADERBOARD_FILE "leaderboard.txt"

// Structures
typedef struct quiz {
    char question[200];
    char answerA[50];
    char answerB[50];
    char answerC[50];
    char answerD[50];
    char correctanswer;
} quiz;

typedef struct user {
    char username[30];
    char password[15];
    int attempts;
    float highest_score;
} user;

typedef struct attempts {
    char username[30];
    char subject[20];
    int totalques;
    float score;
    float percentage;
    char timestamps[50];
} attempts;

#endif