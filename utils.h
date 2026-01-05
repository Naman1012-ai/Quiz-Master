#ifndef UTILS_H
#define UTILS_H

void clearscreen();
void border(char *color);
void centered(char *text, char *color);
void get_timestamps(char *tmstmp);
int safe_input(int min, int max);
void gotoxy(int x, int y);

#endif