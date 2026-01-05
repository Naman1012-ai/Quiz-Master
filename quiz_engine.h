#ifndef QUIZ_ENGINE_H
#define QUIZ_ENGINE_H

#include "common.h"

void display_timer(int seconds, int posy);
char get_validans_with_timer(int *time_left, int timer_pos);
void save_attempts(attempts *att);
void update_leaderboard(char *username, float score, char *timestamps, int numques, char *subject);
void leaderboard();
void shuffleques(quiz *ques, int n);
int diffsub(char *quesfile, char *username, char *subject, int difficulty);
void take_quiz(char *username);
void user_profile(char *username);
void past_attempts(char *username);

#endif